
#include "adc.h"

void adc_init(Adc *ADCX, adc_hw_t *adc_hw, adc_cfg_t *adc_cfg)
{
	/* Enable the APB clock for the ADC */
	PM->APBCMASK.reg |= adc_hw->apbcmask;

	/* Direct GCLK0 to ADC */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(adc_hw->clkctrl_id_val)
					| GCLK_CLKCTRL_CLKEN
					| GCLK_CLKCTRL_GEN(adc_hw->gclk_val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable ADC */
	while (ADCX->STATUS.bit.SYNCBUSY);
	ADCX->CTRLA.bit.ENABLE = 0;

	/* Reset ADC */
	while (ADCX->STATUS.bit.SYNCBUSY);
	ADCX->CTRLA.bit.SWRST = 1;
	while (ADCX->CTRLA.bit.SWRST || ADCX->STATUS.bit.SYNCBUSY);

	/* Get calibration values from NVM */
	uint32_t bias = *(uint32_t *)ADC_FUSES_BIASCAL_ADDR;
	bias = (bias & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;

	uint32_t linearity_0 = *(uint32_t *)ADC_FUSES_LINEARITY_0_ADDR;
	linearity_0 = (linearity_0 & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;

	uint32_t linearity_1 = *(uint32_t *)ADC_FUSES_LINEARITY_1_ADDR;
	linearity_1 = (linearity_1 & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos;

	uint32_t linearity = (linearity_1 << 5) | linearity_0;

	/* Write calibration values to calibration register */
	ADCX->CALIB.reg = ADC_CALIB_BIAS_CAL(bias)
					| ADC_CALIB_LINEARITY_CAL(linearity);

	/* Set reference, enable reference buffer */
	ADCX->REFCTRL.reg = ADC_REFCTRL_REFSEL(adc_cfg->refsel_val)
					| ADC_REFCTRL_REFCOMP;

	/* Set input gain, ground as negative reference */
	ADCX->INPUTCTRL.reg = ADC_INPUTCTRL_GAIN(adc_cfg->gain_val)
						| ADC_INPUTCTRL_MUXPOS_PIN0
						| ADC_INPUTCTRL_MUXNEG_GND;

	/* No averaging */
	ADCX->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1;

	/* 12-bit resolution, set prescaler */
	ADCX->CTRLB.reg = ADC_CTRLB_RESSEL_12BIT
					| ADC_CTRLB_PRESCALER(adc_cfg->prescaler_val);

	/* Enable ADC */
	ADCX->CTRLA.bit.ENABLE = 1;
	while (ADCX->STATUS.bit.SYNCBUSY);
}

uint16_t adc_read_single(Adc *ADCX, uint8_t mux_pos_val, uint8_t mux_neg_val)
{
	/* Flush ADC */
	ADCX->SWTRIG.reg = ADC_SWTRIG_FLUSH;
	while (ADCX->SWTRIG.bit.FLUSH);

	/* Set positive and negative mux */
	ADCX->INPUTCTRL.bit.MUXPOS = mux_pos_val;
	ADCX->INPUTCTRL.bit.MUXNEG = mux_neg_val;
	while (ADCX->STATUS.bit.SYNCBUSY);

	/* Start the ADC */
	ADCX->SWTRIG.bit.START = 1;
	while (!ADCX->INTFLAG.bit.RESRDY);
	ADCX->INTFLAG.reg = ADC_INTFLAG_RESRDY;

	/* Discard first result, repeat */
	ADCX->SWTRIG.bit.START = 1;
	while (!ADCX->INTFLAG.bit.RESRDY);
	ADCX->INTFLAG.reg = ADC_INTFLAG_RESRDY;

	return ADCX->RESULT.reg;
}

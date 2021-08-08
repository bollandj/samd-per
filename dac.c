
#include "dac.h"
#include "gpio.h"
#include "per_cfg.h"

void dac_init()
{
	/* Enable APB clock */
	PM->APBCMASK.reg |= PM_APBCMASK_DAC;

	/* Direct CPU clock (GCLK0) to DAC */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_DAC) |
						GCLK_CLKCTRL_GEN(0) |
						GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable DAC */
	while (DAC->STATUS.bit.SYNCBUSY);
	DAC->CTRLA.bit.ENABLE = 0;

	/* Reset the SERCOM */
	DAC->CTRLA.bit.SWRST = 1;
	while (DAC->STATUS.bit.SYNCBUSY);

	/* Enable pin mux */
	gpio_pin_set_pmux(DAC_VOUT_PIN, DAC_VOUT_PMUX_Val);

	/* Enable external output, select AVCC as reference */
	DAC->CTRLB.reg = DAC_CTRLB_EOEN | DAC_CTRLB_REFSEL(DAC_REFSEL_Val);

	/* Enable DAC */
	DAC->CTRLA.bit.ENABLE = 1;
	while (DAC->STATUS.bit.SYNCBUSY);
}

void dac_write(uint16_t val)
{
	while (DAC->STATUS.bit.SYNCBUSY);
	DAC->DATABUF.reg = val;
}

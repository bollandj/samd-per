
#include "dac.h"

void dac_init(Dac *DACX, dac_hw_t *dac_hw, dac_cfg_t *dac_cfg)
{
	/* Enable APB clock */
	PM->APBCMASK.reg |= dac_hw->apbcmask;

	/* Direct selected GCLK to DAC */
	/* NOTE: max. DAC GCLK frequency is 350kHz! */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(dac_hw->clkctrl_id_val)
					| GCLK_CLKCTRL_CLKEN
					| GCLK_CLKCTRL_GEN(dac_hw->gclk_val);

	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable DAC */
	while (DACX->STATUS.bit.SYNCBUSY);
	DACX->CTRLA.bit.ENABLE = 0;

	/* Reset DAC */
	DACX->CTRLA.bit.SWRST = 1;
	while (DACX->STATUS.bit.SYNCBUSY);

	/* Enable external output */
	DACX->CTRLB.reg = DAC_CTRLB_EOEN
					| (dac_cfg->leftadj ? DAC_CTRLB_LEFTADJ : 0)
					| DAC_CTRLB_REFSEL(dac_cfg->refsel);

	/* Enable DAC */
	DACX->CTRLA.bit.ENABLE = 1;
	while (DACX->STATUS.bit.SYNCBUSY);;
}

void dac_write(Dac *DACX, uint16_t val)
{
	while (DACX->STATUS.bit.SYNCBUSY);
	DACX->DATABUF.reg = val;
}

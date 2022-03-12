
#include "i2sm.h"

void i2sm_init(I2s *I2SX, i2sm_hw_t *i2sm_hw, i2sm_cfg_t *i2sm_cfg)
{
	/* Enable APB clock for corresponding I2S with Power Manager */
	PM->APBCMASK.reg |= i2sm_hw->apbcmask;

	/* Direct selected GCLK to I2S */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(i2sm_hw->clkctrl_id_val)
						| GCLK_CLKCTRL_CLKEN
						| GCLK_CLKCTRL_GEN(i2sm_hw->gclk_val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable I2S */
	while (I2SX->SYNCBUSY.bit.ENABLE);
	I2SX->CTRLA.bit.ENABLE = 0;

	/* Wait for sync, then reset I2S */
	while (I2SX->CTRLA.bit.SWRST);
	I2SX->CTRLA.bit.SWRST = 1;
	while (I2SX->SYNCBUSY.bit.SWRST || I2SX->SYNCBUSY.bit.ENABLE);

	/* Enable I2S */
	I2SX->CTRLA.bit.ENABLE = 1;
	while (I2SX->SYNCBUSY.bit.ENABLE);
}
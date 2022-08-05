
#include "i2cs.h"
#include "gpio.h"

void i2cs_init(Sercom *SERCOMX, i2cs_hw_t *i2cs_hw, uint8_t i2cs_addr)
{
	/* Enable APB clock for corresponding SERCOM */
	PM->APBCMASK.reg |= i2cs_hw->apbcmask;

	/* Direct GCLK0 (system clock) to SERCOM */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(i2cs_hw->clkctrl_id_val)
						| GCLK_CLKCTRL_GEN(i2cs_hw->gclk_val)
						| GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable SERCOM */
	while (SERCOMX->I2CS.SYNCBUSY.bit.ENABLE);
	SERCOMX->I2CS.CTRLA.bit.ENABLE = 0;

	/* Wait for sync, then reset SERCOM */
	while (SERCOMX->I2CS.CTRLA.bit.SWRST);
	SERCOMX->I2CS.CTRLA.bit.SWRST = 1;
	while (SERCOMX->I2CS.SYNCBUSY.bit.SWRST || SERCOMX->I2CS.SYNCBUSY.bit.ENABLE);

	/* Configure as slave, SCLSM = 0, SDA = PAD[0], SCK = PAD[1] */
	SERCOMX->I2CS.CTRLA.reg = SERCOM_I2CS_CTRLA_MODE_I2C_SLAVE;
	
	/* Enable smart operation */
	SERCOMX->I2CS.CTRLB.reg = SERCOM_I2CS_CTRLB_SMEN;

	/* Set address */
	SERCOMX->I2CS.ADDR.reg = SERCOM_I2CS_ADDR_ADDR(i2cs_addr);			

	/* Wait for sync, then enable */
	while (SERCOMX->I2CS.SYNCBUSY.bit.ENABLE);								
	SERCOMX->I2CS.CTRLA.reg |= SERCOM_I2CS_CTRLA_ENABLE;

	/* Set interrupt mask and enable interrupts */
	SERCOMX->I2CS.INTENSET.reg = 
			SERCOM_I2CS_INTENSET_PREC |												
			SERCOM_I2CS_INTENSET_AMATCH |											
			SERCOM_I2CS_INTENSET_DRDY;											
}

void i2cs_enable(IRQn_Type SERCOMX_IRQn)
{
	NVIC_EnableIRQ(SERCOMX_IRQn);
}

void i2cs_disable(IRQn_Type SERCOMX_IRQn)
{
	NVIC_DisableIRQ(SERCOMX_IRQn);
}




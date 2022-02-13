
#include "i2cm.h"
#include "gpio.h"

void i2cm_init(Sercom *SERCOMX, i2cm_hw_t *i2cm_hw, i2cm_cfg_t *i2cm_cfg)
{
	/* Enable APB clock for corresponding SERCOM */
	PM->APBCMASK.reg |= i2cm_hw->apbcmask;

	/* Direct GCLK0 (system clock) to SERCOM */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(i2cm_hw->clkctrl_id_val)
						| GCLK_CLKCTRL_GEN(i2cm_hw->gclk_val)
						| GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable SERCOM */
	while (SERCOMX->I2CM.SYNCBUSY.bit.ENABLE);
	SERCOMX->I2CM.CTRLA.bit.ENABLE = 0;

	/* Wait for sync, then reset SERCOM */
	while (SERCOMX->I2CM.CTRLA.bit.SWRST);
	SERCOMX->I2CM.CTRLA.bit.SWRST = 1;
	while (SERCOMX->I2CM.SYNCBUSY.bit.SWRST || SERCOMX->I2CM.SYNCBUSY.bit.ENABLE);

	/* Configure as master, SDA = PAD[0], SCK = PAD[1] */
	SERCOMX->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_SPEED(i2cm_cfg->speed)
								  | SERCOM_I2CM_CTRLA_SDAHOLD(i2cm_cfg->sda_hold_time)
								  | SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;

	/* Enable smart operation, quick command */
	SERCOMX->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN | SERCOM_I2CM_CTRLB_QCEN;
	while (SERCOMX->I2CM.SYNCBUSY.bit.SYSOP);

	/* Set baud rate */
	SERCOMX->I2CM.BAUD.reg = SystemCoreClock / (2 * i2cm_cfg->baud) - 5;

	/* Enable SERCOM */
	SERCOMX->I2CM.CTRLA.bit.ENABLE = 1;
	while (SERCOMX->I2CM.SYNCBUSY.bit.ENABLE);

	/* Put bus into idle state */
	SERCOMX->I2CM.STATUS.bit.BUSSTATE = I2CM_BUSSTATE_IDLE;
	while (SERCOMX->I2CM.SYNCBUSY.bit.SYSOP);
}

int8_t i2cm_start(Sercom *SERCOMX, uint8_t address)
{
	if (SERCOMX->I2CM.STATUS.bit.BUSSTATE == I2CM_BUSSTATE_BUSY)
		return -1;

	/* Write address register */
	SERCOMX->I2CM.ADDR.bit.ADDR = address << 1 | I2CM_WRITE;
	while (SERCOMX->I2CM.SYNCBUSY.bit.SYSOP);

	/* Wait for Master on Bus interrupt flag */
	while (!SERCOMX->I2CM.INTFLAG.bit.MB);

	/* Don't continue transfer if not acknowledged */
	if (SERCOMX->I2CM.STATUS.bit.RXNACK)
		return -1;

	return 0;
}

int8_t i2cm_write(Sercom *SERCOMX, const uint8_t *data, uint32_t len)
{
	/* Write data */
	for (uint8_t i = 0; i < len; i++)
	{
		SERCOMX->I2CM.DATA.reg = data[i];
		while (SERCOMX->I2CM.SYNCBUSY.bit.SYSOP);

		while (!SERCOMX->I2CM.INTFLAG.bit.MB);

		if (SERCOMX->I2CM.STATUS.bit.RXNACK)
			return -1;
	}

	return 0;
}

int8_t i2cm_write_single(Sercom *SERCOMX, uint8_t data)
{
	/* Write data */
	SERCOMX->I2CM.DATA.reg = data;
	while (SERCOMX->I2CM.SYNCBUSY.bit.SYSOP);

	while (!SERCOMX->I2CM.INTFLAG.bit.MB);

	if (SERCOMX->I2CM.STATUS.bit.RXNACK)
		return -1;

	return 0;
}

void i2cm_stop(Sercom *SERCOMX)
{
	SERCOMX->I2CM.CTRLB.bit.CMD = I2CM_CMD_STOP_COND;
	while (SERCOMX->I2CM.SYNCBUSY.bit.SYSOP);
}

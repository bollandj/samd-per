
#include "i2cm.h"
#include "gpio.h"
#include "per_cfg.h"

void i2cm_init()
{
	/* Enable APB clock for corresponding SERCOM */
	PM->APBCMASK.reg |= I2CM_SERCOM_APBCMASK;

	/* Direct GCLK0 (system clock) to SERCOM */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(I2CM_SERCOM_CLKCTRL_ID_CORE_Val)
						| GCLK_CLKCTRL_GEN(0)
						| GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable SERCOM */
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.ENABLE);
	I2CM_SERCOM->I2CM.CTRLA.bit.ENABLE = 0;

	/* Wait for sync, then reset SERCOM */
	while (I2CM_SERCOM->I2CM.CTRLA.bit.SWRST);
	I2CM_SERCOM->I2CM.CTRLA.bit.SWRST = 1;
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SWRST || I2CM_SERCOM->I2CM.SYNCBUSY.bit.ENABLE);

	/* Set up pin mux */
	gpio_pin_set_pmux(I2CM_SDA_PIN, I2CM_SDA_PMUX_Val);
	gpio_pin_set_pmux(I2CM_SCL_PIN, I2CM_SCL_PMUX_Val);

	/* Configure as master, 100/400kHz, SDA = PAD[0], SCK = PAD[1] */
	I2CM_SERCOM->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_SPEED(0)
								  | SERCOM_I2CM_CTRLA_SDAHOLD(0)
								  | SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;

	/* Enable smart operation, quick command */
	I2CM_SERCOM->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN | SERCOM_I2CM_CTRLB_QCEN;
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SYSOP);

	/* Set baud rate */
	I2CM_SERCOM->I2CM.BAUD.reg = SystemCoreClock / (2 * I2CM_BAUD) - 5;

	/* Enable SERCOM */
	I2CM_SERCOM->I2CM.CTRLA.bit.ENABLE = 1;
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.ENABLE);

	/* Put bus into idle state */
	I2CM_SERCOM->I2CM.STATUS.bit.BUSSTATE = I2CM_BUSSTATE_IDLE;
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SYSOP);
}

int8_t i2cm_start(uint8_t address)
{
	if (I2CM_SERCOM->I2CM.STATUS.bit.BUSSTATE == I2CM_BUSSTATE_BUSY)
		return -1;

	/* Write address register */
	I2CM_SERCOM->I2CM.ADDR.bit.ADDR = address << 1 | I2CM_WRITE;
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SYSOP);

	/* Wait for Master on Bus interrupt flag */
	while (!I2CM_SERCOM->I2CM.INTFLAG.bit.MB);

	/* Don't continue transfer if not acknowledged */
	if (I2CM_SERCOM->I2CM.STATUS.bit.RXNACK)
		return -1;

	return 0;
}

int8_t i2cm_write(const uint8_t *data, uint32_t len)
{
	/* Write data */
	for (uint8_t i = 0; i < len; i++)
	{
		I2CM_SERCOM->I2CM.DATA.reg = data[i];
		while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SYSOP);

		while (!I2CM_SERCOM->I2CM.INTFLAG.bit.MB);

		if (I2CM_SERCOM->I2CM.STATUS.bit.RXNACK)
			return -1;
	}

	return 0;
}

int8_t i2cm_write_single(uint8_t data)
{
	/* Write data */
	I2CM_SERCOM->I2CM.DATA.reg = data;
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SYSOP);

	while (!I2CM_SERCOM->I2CM.INTFLAG.bit.MB);

	if (I2CM_SERCOM->I2CM.STATUS.bit.RXNACK)
		return -1;

	return 0;
}

void i2cm_stop()
{
	I2CM_SERCOM->I2CM.CTRLB.bit.CMD = I2CM_CMD_STOP_COND;
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SYSOP);
}

int8_t i2cm_transfer(uint8_t address, const uint8_t *write_buf, uint8_t write_len, uint8_t *read_buf, uint8_t read_len)
{
	i2cm_start(address);

	/* Write data */
	for (uint8_t i = 0; i < write_len; i++)
	{
		I2CM_SERCOM->I2CM.DATA.reg = write_buf[i];
		while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SYSOP);

		while (!I2CM_SERCOM->I2CM.INTFLAG.bit.MB);

		if (I2CM_SERCOM->I2CM.STATUS.bit.RXNACK)
			return -1;
	}

	/* End by issuing stop condition */
	I2CM_SERCOM->I2CM.CTRLB.bit.CMD = I2CM_CMD_STOP_COND;
	while (I2CM_SERCOM->I2CM.SYNCBUSY.bit.SYSOP);

	return 0;
}

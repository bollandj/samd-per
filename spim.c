/*
 * spi.c
 *
 * Created: 11/05/2021 22:04:15
 *  Author: Joshua
 */

#include "spim.h"
#include "gpio.h"
#include "per_cfg.h"

void spim_init()
{
	/* Enable APB clock for corresponding SERCOM with Power Manager */
	PM->APBCMASK.reg |= SPIM_SERCOM_APBCMASK;

	/* Direct GCLK0 (system clock) to SERCOM */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SPIM_SERCOM_CLKCTRL_ID_CORE_Val) |
						GCLK_CLKCTRL_GEN(0) |
						GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable SERCOM */
	while (SPIM_SERCOM->SPI.SYNCBUSY.bit.ENABLE);
	SPIM_SERCOM->SPI.CTRLA.bit.ENABLE = 0;

	/* Wait for sync, then reset SERCOM */
	while (SPIM_SERCOM->SPI.CTRLA.bit.SWRST);
	SPIM_SERCOM->SPI.CTRLA.bit.SWRST = 1;
	while (SPIM_SERCOM->SPI.SYNCBUSY.bit.SWRST || SPIM_SERCOM->SPI.SYNCBUSY.bit.ENABLE);

	/* Set up pin mux */
	gpio_pin_set_pmux(SPIM_SCK_PIN, SPIM_SCK_PMUX_Val);
	gpio_pin_set_pmux(SPIM_SDO_PIN, SPIM_SDO_PMUX_Val);

	/* Configure as master, DO = PAD[0], SCK = PAD[3] */
	SPIM_SERCOM->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPIM_MASTER |
								SERCOM_SPI_CTRLA_DOPO(3);

	/* Set SPI mode (clock polarity + phase) */
	SPIM_SERCOM->SPI.CTRLA.bit.CPOL = SPIM_POLARITY;
	SPIM_SERCOM->SPI.CTRLA.bit.CPHA = SPIM_PHASE;

	/* Set baud rate */
	SPIM_SERCOM->SPI.BAUD.reg = SPIM_GCLK_FREQ / (2 * SPIM_BAUD) - 1;

	/* Hardware Slave Select */
	//SPIM_SERCOM->SPI.CTRLB.bit.MSSEN = 1;
	//while (SPIM_SERCOM->SPI.SYNCBUSY.bit.CTRLB);

	/* Enable SERCOM */
	SPIM_SERCOM->SPI.CTRLA.bit.ENABLE = 1;
	while (SPIM_SERCOM->SPI.SYNCBUSY.bit.ENABLE);
}

void spim_write(const uint8_t *data, uint32_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		while (!SPIM_SERCOM->SPI.INTFLAG.bit.DRE);
		SPIM_SERCOM->SPI.DATA.reg = data[i];
	}
	/* Wait for final byte to be fully sent to facilitate software chip select */
	while (!SPIM_SERCOM->SPI.INTFLAG.bit.TXC);
}

void spim_write_single(uint8_t data)
{
	while (!SPIM_SERCOM->SPI.INTFLAG.bit.DRE);
	SPIM_SERCOM->SPI.DATA.reg = data;
	/* Wait for byte to be fully sent to facilitate software chip select */
	while (!SPIM_SERCOM->SPI.INTFLAG.bit.TXC);
}
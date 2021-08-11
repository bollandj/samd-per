/*
 * spi.c
 *
 * Created: 11/05/2021 22:04:15
 *  Author: Joshua
 */

#include "spi.h"
#include "gpio.h"
#include "per_cfg.h"

void spi_init()
{
	/* Enable APB clock for corresponding SERCOM with Power Manager */
	PM->APBCMASK.reg |= SPI_SERCOM_APBCMASK;

	/* Direct GCLK0 (system clock) to SERCOM */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SPI_SERCOM_CLKCTRL_ID_CORE_Val) |
						GCLK_CLKCTRL_GEN(0) |
						GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable SERCOM */
	while (SPI_SERCOM->SPI.SYNCBUSY.bit.ENABLE);
	SPI_SERCOM->SPI.CTRLA.bit.ENABLE = 0;

	/* Wait for sync, then reset SERCOM */
	while (SPI_SERCOM->SPI.CTRLA.bit.SWRST);
	SPI_SERCOM->SPI.CTRLA.bit.SWRST = 1;
	while (SPI_SERCOM->SPI.SYNCBUSY.bit.SWRST || SPI_SERCOM->SPI.SYNCBUSY.bit.ENABLE);

	/* Set up pin mux */
	gpio_pin_set_pmux(SPI_SCK_PIN, SPI_SCK_PMUX_Val);
	gpio_pin_set_pmux(SPI_SDO_PIN, SPI_SDO_PMUX_Val);

	/* Configure as master, DO = PAD[0], SCK = PAD[3] */
	SPI_SERCOM->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPI_MASTER |
								SERCOM_SPI_CTRLA_DOPO(3);

	/* Set SPI mode (clock polarity + phase) */
	SPI_SERCOM->SPI.CTRLA.bit.CPOL = SPI_POLARITY;
	SPI_SERCOM->SPI.CTRLA.bit.CPHA = SPI_PHASE;

	/* Set baud rate */
	SPI_SERCOM->SPI.BAUD.reg = SPI_GCLK_FREQ / (2 * SPI_BAUD) - 1;

	/* Hardware Slave Select */
	//SPI_SERCOM->SPI.CTRLB.bit.MSSEN = 1;
	//while (SPI_SERCOM->SPI.SYNCBUSY.bit.CTRLB);

	/* Enable SERCOM */
	SPI_SERCOM->SPI.CTRLA.bit.ENABLE = 1;
	while (SPI_SERCOM->SPI.SYNCBUSY.bit.ENABLE);
}

void spi_write(const uint8_t *data, uint32_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		while (!SPI_SERCOM->SPI.INTFLAG.bit.DRE);
		SPI_SERCOM->SPI.DATA.reg = data[i];
	}
	/* Wait for final byte to be fully sent to facilitate software chip select */
	while (!SPI_SERCOM->SPI.INTFLAG.bit.TXC);
}

void spi_write_single(uint8_t data)
{
	while (!SPI_SERCOM->SPI.INTFLAG.bit.DRE);
	SPI_SERCOM->SPI.DATA.reg = data;
	/* Wait for byte to be fully sent to facilitate software chip select */
	while (!SPI_SERCOM->SPI.INTFLAG.bit.TXC);
}
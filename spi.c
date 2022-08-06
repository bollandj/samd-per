/*
 * spi.c
 *
 * Created: 11/05/2021 22:04:15
 *  Author: Joshua
 */

#include "spi.h"
#include "gpio.h"

void spi_init(Sercom *SERCOMX, spi_hw_t *spi_hw, spi_cfg_t *spi_cfg, spi_pinout_t *spi_pinout)
{
	/* Enable APB clock for corresponding SERCOM with Power Manager */
	PM->APBCMASK.reg |= spi_hw->apbcmask;

	/* Direct selected GCLK to SERCOM */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(spi_hw->clkctrl_id_val)
					| GCLK_CLKCTRL_CLKEN
					| GCLK_CLKCTRL_GEN(spi_hw->gclk_val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable SERCOM */
	while (SERCOMX->SPI.SYNCBUSY.bit.ENABLE);
	SERCOMX->SPI.CTRLA.bit.ENABLE = 0;

	/* Wait for sync, then reset SERCOM */
	while (SERCOMX->SPI.CTRLA.bit.SWRST);
	SERCOMX->SPI.CTRLA.bit.SWRST = 1;
	while (SERCOMX->SPI.SYNCBUSY.bit.SWRST || SERCOMX->SPI.SYNCBUSY.bit.ENABLE);

	/* Configure as master, set pads for SCK, DI, DO */
	SERCOMX->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPI_MASTER
							| SERCOM_SPI_CTRLA_DOPO(spi_pinout->dopo_val)
							| SERCOM_SPI_CTRLA_DIPO(spi_pinout->dipo_val);

	/* Set SPI mode (clock polarity + phase), MSB/LSB first */
	SERCOMX->SPI.CTRLA.bit.CPOL = spi_cfg->polarity;
	SERCOMX->SPI.CTRLA.bit.CPHA = spi_cfg->phase;
	SERCOMX->SPI.CTRLA.bit.DORD = spi_cfg->lsb_first;

	/* Set baud rate */
	SERCOMX->SPI.BAUD.reg = SystemCoreClock / (2 * spi_cfg->baud) - 1;

	/* Hardware Slave Select */
	//SERCOMX->SPI.CTRLB.bit.MSSEN = 1;
	//while (SERCOMX->SPI.SYNCBUSY.bit.CTRLB);

	/* Enable SERCOM */
	SERCOMX->SPI.CTRLA.bit.ENABLE = 1;
	while (SERCOMX->SPI.SYNCBUSY.bit.ENABLE);
}

void spi_write(Sercom *SERCOMX, const uint8_t *data, uint32_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		while (!SERCOMX->SPI.INTFLAG.bit.DRE);
		SERCOMX->SPI.DATA.reg = data[i];
	}
	/* Wait for final byte to be fully sent to facilitate software chip select */
	while (!SERCOMX->SPI.INTFLAG.bit.TXC);
}

void spi_write_single(Sercom *SERCOMX, uint8_t data)
{
	while (!SERCOMX->SPI.INTFLAG.bit.DRE);
	SERCOMX->SPI.DATA.reg = data;
	/* Wait for byte to be fully sent to facilitate software chip select */
	while (!SERCOMX->SPI.INTFLAG.bit.TXC);
}
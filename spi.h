/*
 * spi.h
 *
 * Created: 11/05/2021 22:04:28
 *  Author: Joshua
 */

#ifndef SPI_H_
#define SPI_H_

#include "sam.h"

/* Which hardware should be used for this SPI instance? */
#define SPI_SERCOM                     SERCOM0                          // Which SERCOM? (SERCOM0-2)
#define SPI_SERCOM_APBCMASK            PM_APBCMASK_SERCOM0              // APB clock enable mask for this SERCOM
#define SPI_SERCOM_CLKCTRL_ID_CORE_Val GCLK_CLKCTRL_ID_SERCOM0_CORE_Val // CLKCTRL ID for SERCOM peripheral (0x0F for SERCOM1)

/* SPI settings */
#define SPI_POLARITY 0
#define SPI_PHASE 0
#define SPI_BAUD 100000UL
#define SPI_GCLK_FREQ 8000000UL

void spi_init();
void spi_write(const uint8_t *data, uint32_t len);
void spi_write_single(uint8_t data);

#endif /* SPI_H_ */
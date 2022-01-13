/*
 * spim.h
 *
 * Created: 11/05/2021 22:04:28
 *  Author: Joshua
 */

#ifndef SPI_H_
#define SPI_H_

#include "sam.h"

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} spi_hw_t;

#define SPI_HW_SERCOM0 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM0,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM0_CORE_Val	\
}

#define SPI_HW_SERCOM1 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM1,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM1_CORE_Val	\
}

#define SPI_HW_SERCOM2 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM2,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM2_CORE_Val	\
}

#define SPI_HW_SERCOM3 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM3,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM3_CORE_Val	\
}

#define SPI_HW_SERCOM4 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM4,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM4_CORE_Val	\
}

#define SPI_HW_SERCOM5 									\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM5,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM5_CORE_Val	\
}

typedef struct
{
	uint32_t baud;
	uint8_t polarity;
	uint8_t phase;
	uint8_t lsb_first;
} spi_cfg_t;

#define SPI_CFG_DEFAULT	\
{ 						\
	.baud = 115200,		\
	.polarity = 0,		\
	.phase = 0,			\
	.lsb_first = 0		\
}

#define SPI_DOPO_DO_PAD0_SCK_PAD1 0
#define SPI_DOPO_DO_PAD2_SCK_PAD3 1
#define SPI_DOPO_DO_PAD3_SCK_PAD1 2
#define SPI_DOPO_DO_PAD0_SCK_PAD3 3

#define SPI_DIPO_DI_NONE 0
#define SPI_DIPO_DI_PAD0 0
#define SPI_DIPO_DI_PAD1 1
#define SPI_DIPO_DI_PAD2 2
#define SPI_DIPO_DI_PAD3 3

typedef struct
{
	uint8_t dopo_val;
	uint8_t dipo_val;
	uint32_t ss_port;
} spi_pinout_t;

#define SPI_PINOUT_DEFAULT					\
{ 											\
	.dopo_val = SPI_DOPO_DO_PAD0_SCK_PAD1, 	\
	.dipo_val = SPI_DIPO_DI_PAD2,			\
	.ss_port  = PORT_PA09               	\
}

void spi_init(Sercom *SERCOMX, spi_hw_t *spi_hw, spi_cfg_t *spi_cfg, spi_pinout_t *spi_pinout);
void spi_write(Sercom *SERCOMX, const uint8_t *data, uint32_t len);
void spi_write_single(Sercom *SERCOMX, uint8_t data);

#endif /* SPI_H_ */
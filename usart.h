
#ifndef USART_H_
#define USART_H_

#include "sam.h"

typedef struct
{
	uint8_t gclk_val;
	uint32_t apbcmask;
	uint32_t clkctrl_id_val;
} usart_hw_t;

#define USART_HW_SERCOM0 								\
{														\
	.gclk_val = CLOCK_GCLK0,							\
	.apbcmask = PM_APBCMASK_SERCOM0,					\
	.clkctrl_id_val = GCLK_CLKCTRL_ID_SERCOM0_CORE_Val	\
}

typedef struct
{
	uint32_t baud;
	uint8_t lsb_first;
} usart_cfg_t;

#define USART_CFG_DEFAULT	\
{ 							\
	.baud = 115200, 		\
	.lsb_first = 1			\
}

#define USART_TXPO_PAD0 0
#define USART_TXPO_PAD2 1

#define USART_RXPO_PAD0 0
#define USART_RXPO_PAD1 1
#define USART_RXPO_PAD2 2
#define USART_RXPO_PAD3 3

typedef struct
{
	uint8_t txpo_val;
	uint8_t rxpo_val;
} usart_pinout_t;

#define USART_PINOUT_DEFAULT		\
{ 									\
	.txpo_val = USART_TXPO_PAD0, 	\
	.rxpo_val = USART_RXPO_PAD1		\
}

void usart_init(Sercom *SERCOMX, usart_hw_t *usart_hw, usart_cfg_t *usart_cfg, usart_pinout_t *usart_pinout);

void usart_enable_interrupts(Sercom *SERCOMX, IRQn_Type SERCOMX_IRQn, uint8_t interrupt_mask);
void usart_disable_interrupts(Sercom *SERCOMX, IRQn_Type SERCOMX_IRQn);

void usart_write(Sercom *SERCOMX, const uint8_t *data, uint32_t len);
void usart_write_single(Sercom *SERCOMX, uint8_t data);
void usart_read(Sercom *SERCOMX, uint8_t *data, uint32_t len);

#endif /* USART_H_ */


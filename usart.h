

#ifndef USART_H_
#define USART_H_

#include "sam.h"

/* Which hardware should be used for this USART instance? */
#define USART_SERCOM                     SERCOM1                          // Which SERCOM? (SERCOM0-2)
#define USART_SERCOM_APBCMASK            PM_APBCMASK_SERCOM1              // APB clock enable mask for this SERCOM
#define USART_SERCOM_CLKCTRL_ID_CORE_Val GCLK_CLKCTRL_ID_SERCOM1_CORE_Val // CLKCTRL ID for SERCOM peripheral (0x0E for SERCOM0)

/* Usart settings */
#define USART_BAUD 9600
#define USART_LSB_FIRST 1

void usart_init();
void usart_write(const uint8_t *data, uint32_t len);
void usart_write_single(uint8_t data);
void usart_read(uint8_t *data, uint32_t len);

#endif /* USART_H_ */


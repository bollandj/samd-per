

#ifndef USART_H_
#define USART_H_

#include "sam.h"

void usart_init();
void usart_write(const uint8_t *data, uint32_t len);
void usart_write_single(uint8_t data);
void usart_read(uint8_t *data, uint32_t len);

#endif /* USART_H_ */


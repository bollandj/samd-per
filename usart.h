

#ifndef USART_H_
#define USART_H_

#include "sam.h"
#include "per_cfg.h"

void usart_init();
void usart_enable_interrupts(uint8_t interrupt_mask);

void usart_write(const uint8_t *data, uint32_t len);
void usart_write_single(uint8_t data);
void usart_read(uint8_t *data, uint32_t len);

#endif /* USART_H_ */


/*
 * gpio.h
 *
 * Created: 11/05/2021 16:33:02
 *  Author: Joshua
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "sam.h"

#define GROUP_PORTA 0 // PORTA group number
#define GROUP_PORTB 1 // PORTB group number

void gpio_port_set_input(uint32_t port_mask);
void gpio_port_set_pullup(uint32_t port_mask);
void gpio_port_set_output(uint32_t port_mask);

inline void gpio_port_set(uint32_t port_mask) { PORT->Group[GROUP_PORTA].OUTSET.reg = port_mask; }
inline void gpio_port_clear(uint32_t port_mask) { PORT->Group[GROUP_PORTA].OUTCLR.reg = port_mask; }
inline void gpio_port_toggle(uint32_t port_mask) { PORT->Group[GROUP_PORTA].OUTTGL.reg = port_mask; }

void gpio_pin_set_input(uint8_t pin_no);
void gpio_pin_set_pullup(uint8_t pin_no);
void gpio_pin_set_output(uint8_t pin_no);

inline void gpio_pin_set(uint8_t pin_no) { PORT->Group[GROUP_PORTA].OUTSET.reg = 1 << pin_no; }
inline void gpio_pin_clear(uint8_t pin_no) { PORT->Group[GROUP_PORTA].OUTCLR.reg = 1 << pin_no; }
inline void gpio_pin_toggle(uint8_t pin_no) { PORT->Group[GROUP_PORTA].OUTTGL.reg = 1 << pin_no; }

void gpio_pin_set_pmux(uint8_t pin_no, uint8_t mux_val);
void gpio_pin_clear_pmux(uint8_t pin_no);


#endif /* GPIO_H_ */

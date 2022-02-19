/*
 * gpio.h
 *
 * Created: 11/05/2021 16:33:02
 *  Author: Joshua
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "sam.h"

#define GPIO_GROUP_PORTA 0 // PORTA group number
#define GPIO_GROUP_PORTB 1 // PORTB group number

void gpio_port_set_input(uint8_t port_group, uint32_t port_mask);
void gpio_port_set_pullup(uint8_t port_group, uint32_t port_mask);
void gpio_port_set_output(uint8_t port_group, uint32_t port_mask);

inline void gpio_port_set(uint8_t port_group, uint32_t port_mask) { PORT->Group[port_group].OUTSET.reg = port_mask; }
inline void gpio_port_clear(uint8_t port_group, uint32_t port_mask) { PORT->Group[port_group].OUTCLR.reg = port_mask; }
inline void gpio_port_toggle(uint8_t port_group, uint32_t port_mask) { PORT->Group[port_group].OUTTGL.reg = port_mask; }
inline void gpio_port_write(uint8_t port_group, uint32_t port_mask, uint32_t port_value) { PORT->Group[port_group].OUT.reg = (PORT->Group[port_group].OUT.reg & ~(1 << port_mask)) \
                                                                                                                           | (port_mask & port_value); }

inline uint32_t gpio_port_read(uint8_t port_group, uint32_t port_mask) { return PORT->Group[port_group].IN.reg & port_mask; }

void gpio_pin_set_input(uint8_t port_group, uint8_t pin_no);
void gpio_pin_set_pullup(uint8_t port_group, uint8_t pin_no);
void gpio_pin_set_output(uint8_t port_group, uint8_t pin_no);

inline void gpio_pin_set(uint8_t port_group, uint8_t pin_no) { PORT->Group[port_group].OUTSET.reg = 1 << pin_no; }
inline void gpio_pin_clear(uint8_t port_group, uint8_t pin_no) { PORT->Group[port_group].OUTCLR.reg = 1 << pin_no; }
inline void gpio_pin_toggle(uint8_t port_group, uint8_t pin_no) { PORT->Group[port_group].OUTTGL.reg = 1 << pin_no; }
inline void gpio_pin_write(uint8_t port_group, uint8_t pin_no, uint8_t pin_value) { PORT->Group[port_group].OUT.reg = (PORT->Group[port_group].OUT.reg & ~(1 << pin_no)) \
                                                                                                                    | (pin_value << pin_no); }

inline uint8_t gpio_pin_read(uint8_t port_group, uint8_t pin_no) { return (PORT->Group[port_group].IN.reg >> pin_no) & 1;}

void gpio_pin_set_pmux(uint8_t port_group, uint8_t pin_no, uint8_t mux_val);
void gpio_pin_clear_pmux(uint8_t port_group, uint8_t pin_no);


#endif /* GPIO_H_ */

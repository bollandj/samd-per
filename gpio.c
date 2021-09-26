/*
 * gpio.c
 *
 * Created: 11/05/2021 16:32:54
 *  Author: Joshua
 */

#include "gpio.h"

void gpio_port_set_input(uint32_t port_mask)
{
	PORT->Group[GROUP_PORTA].DIRCLR.reg = port_mask;

	for (uint8_t i = 0; i < 32; i++)
		if (port_mask & 1 << i)
			PORT->Group[GROUP_PORTA].PINCFG[i].bit.INEN = 1;
}

void gpio_port_set_pullup(uint32_t port_mask)
{
	for (uint8_t i = 0; i < 32; i++)
		if (port_mask & 1 << i)
			PORT->Group[GROUP_PORTA].PINCFG[i].bit.PULLEN = 1;

	PORT->Group[GROUP_PORTA].OUTSET.reg = port_mask;
}

void gpio_port_set_output(uint32_t port_mask)
{
	PORT->Group[GROUP_PORTA].OUTCLR.reg = port_mask;
	PORT->Group[GROUP_PORTA].DIRSET.reg = port_mask;
}

void gpio_pin_set_input(uint8_t pin_no)
{
	PORT->Group[GROUP_PORTA].DIRCLR.reg = 1 << pin_no;
	PORT->Group[GROUP_PORTA].PINCFG[pin_no].bit.INEN = 1;
}

void gpio_pin_set_pullup(uint8_t pin_no)
{
	PORT->Group[GROUP_PORTA].PINCFG[pin_no].bit.PULLEN = 1;
	PORT->Group[GROUP_PORTA].OUTSET.reg = 1 << pin_no;
}

void gpio_pin_set_output(uint8_t pin_no)
{
	PORT->Group[GROUP_PORTA].OUTCLR.reg = 1 << pin_no;
	PORT->Group[GROUP_PORTA].DIRSET.reg = 1 << pin_no;
}

void gpio_pin_set_pmux(uint8_t pin_no, uint8_t mux_val)
{
	/* Enable alternate pin function via pin mux */
	PORT->Group[GROUP_PORTA].PINCFG[pin_no].bit.PMUXEN = 1;

	/* Odd or even */
	if (pin_no & 1)
		PORT->Group[GROUP_PORTA].PMUX[pin_no >> 1].bit.PMUXO = mux_val;
	else
		PORT->Group[GROUP_PORTA].PMUX[pin_no >> 1].bit.PMUXE = mux_val;
}

void gpio_pin_clear_pmux(uint8_t pin_no)
{
	PORT->Group[GROUP_PORTA].PINCFG[pin_no].bit.PMUXEN = 0;
}
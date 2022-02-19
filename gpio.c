/*
 * gpio.c
 *
 * Created: 11/05/2021 16:32:54
 *  Author: Joshua
 */

#include "gpio.h"

void gpio_port_set_input(uint8_t port_group, uint32_t port_mask)
{
	PORT->Group[port_group].DIRCLR.reg = port_mask;

	for (uint8_t i = 0; i < 32; i++)
		if (port_mask & 1 << i)
			PORT->Group[port_group].PINCFG[i].bit.INEN = 1;
}

void gpio_port_set_pullup(uint8_t port_group, uint32_t port_mask)
{
	for (uint8_t i = 0; i < 32; i++)
		if (port_mask & 1 << i)
			PORT->Group[port_group].PINCFG[i].bit.PULLEN = 1;

	PORT->Group[port_group].OUTSET.reg = port_mask;
}

void gpio_port_set_output(uint8_t port_group, uint32_t port_mask)
{
	PORT->Group[port_group].OUTCLR.reg = port_mask;
	PORT->Group[port_group].DIRSET.reg = port_mask;
}

void gpio_pin_set_input(uint8_t port_group, uint8_t pin_no)
{
	PORT->Group[port_group].DIRCLR.reg = 1 << pin_no;
	PORT->Group[port_group].PINCFG[pin_no].bit.INEN = 1;
}

void gpio_pin_set_pullup(uint8_t port_group, uint8_t pin_no)
{
	PORT->Group[port_group].PINCFG[pin_no].bit.PULLEN = 1;
	PORT->Group[port_group].OUTSET.reg = 1 << pin_no;
}

void gpio_pin_set_output(uint8_t port_group, uint8_t pin_no)
{
	PORT->Group[port_group].OUTCLR.reg = 1 << pin_no;
	PORT->Group[port_group].DIRSET.reg = 1 << pin_no;
}

void gpio_pin_set_pmux(uint8_t port_group, uint8_t pin_no, uint8_t mux_val)
{
	/* Enable alternate pin function via pin mux */
	PORT->Group[port_group].PINCFG[pin_no].bit.PMUXEN = 1;

	/* Odd or even */
	if (pin_no & 1)
		PORT->Group[port_group].PMUX[pin_no >> 1].bit.PMUXO = mux_val;
	else
		PORT->Group[port_group].PMUX[pin_no >> 1].bit.PMUXE = mux_val;
}

void gpio_pin_clear_pmux(uint8_t port_group, uint8_t pin_no)
{
	PORT->Group[port_group].PINCFG[pin_no].bit.PMUXEN = 0;
}
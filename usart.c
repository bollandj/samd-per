
#include "usart.h"
#include "gpio.h"
#include "per_cfg.h"

void usart_init()
{
	/* Enable APB clock for corresponding SERCOM with Power Manager */
	PM->APBCMASK.reg |= USART_SERCOM_APBCMASK;

	/* Direct GCLK0 to SERCOM */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(USART_SERCOM_CLKCTRL_ID_CORE_Val) |
						GCLK_CLKCTRL_GEN(0) |
						GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable SERCOM */
	while (USART_SERCOM->USART.SYNCBUSY.bit.ENABLE);
	USART_SERCOM->USART.CTRLA.bit.ENABLE = 0;

	/* Reset the SERCOM */
	while (USART_SERCOM->SPI.CTRLA.bit.SWRST);
	USART_SERCOM->USART.CTRLA.bit.SWRST = 1;
	while (USART_SERCOM->USART.CTRLA.bit.SWRST || USART_SERCOM->USART.SYNCBUSY.bit.SWRST);

	/* Set up pin mux */
	gpio_pin_set_pmux(USART_TX_PIN, USART_TX_PMUX_Val);
	gpio_pin_set_pmux(USART_RX_PIN, USART_RX_PMUX_Val);

	/* Configure MSB/LSB first, 16x oversampling + arithmetic baud rate, pinouts */
	USART_SERCOM->USART.CTRLA.reg = (USART_LSB_FIRST << SERCOM_USART_CTRLA_DORD_Pos) |
									SERCOM_USART_CTRLA_TXPO(USART_TXPO_Val) |
									SERCOM_USART_CTRLA_RXPO(USART_RXPO_Val) |
									SERCOM_USART_CTRLA_SAMPR(0) |
									SERCOM_USART_CTRLA_RUNSTDBY |
									SERCOM_USART_CTRLA_MODE_USART_INT_CLK;

	/* Set baud rate (arithmetic) */
	USART_SERCOM->USART.BAUD.reg = 65536 - ((uint64_t)65536 * 16 * USART_BAUD) / SystemCoreClock;

	/* Enable transmitter and receiver, 8 bit character size */
	USART_SERCOM->USART.CTRLB.reg = SERCOM_USART_CTRLB_CHSIZE(0) |
									SERCOM_USART_CTRLB_TXEN |
									SERCOM_USART_CTRLB_RXEN;
	while (USART_SERCOM->USART.SYNCBUSY.bit.CTRLB);

	/* Enable RX Complete interrupt */
	USART_SERCOM->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;

	/* Enable interrupt handler */
	NVIC_SetPriority(SERCOM1_IRQn, 1 << __NVIC_PRIO_BITS);
	NVIC_EnableIRQ(SERCOM1_IRQn);

	// Enable SERCOM */
	USART_SERCOM->USART.CTRLA.bit.ENABLE = 1;
	while (USART_SERCOM->USART.SYNCBUSY.bit.ENABLE);
}

void usart_write(const uint8_t *data, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
	{
		while (!USART_SERCOM->USART.INTFLAG.bit.DRE);
		USART_SERCOM->USART.DATA.reg = data[i];
	}
}

void usart_write_single(uint8_t data)
{
	while (!USART_SERCOM->USART.INTFLAG.bit.DRE);
	USART_SERCOM->USART.DATA.reg = data;
}

void usart_read(uint8_t *data, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
	{
		while (!USART_SERCOM->USART.INTFLAG.bit.RXC);
		data[i] = USART_SERCOM->USART.DATA.reg;
	}
}
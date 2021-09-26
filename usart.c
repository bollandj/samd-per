
#include "usart.h"

void usart_init(Sercom *SERCOMX, usart_hw_t *usart_hw, usart_cfg_t *usart_cfg, usart_pinout_t *usart_pinout)
{
	/* Enable APB clock for corresponding SERCOM with Power Manager */
	PM->APBCMASK.reg |= usart_hw->apbcmask;

	/* Clock SERCOM with selected GCLK */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(usart_hw->clkctrl_id_val)
					| GCLK_CLKCTRL_CLKEN
					| GCLK_CLKCTRL_GEN(usart_hw->gclk_val);
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Wait for sync, then disable SERCOM */
	while (SERCOMX->USART.SYNCBUSY.bit.ENABLE);
	SERCOMX->USART.CTRLA.bit.ENABLE = 0;

	/* Reset the SERCOM */
	while (SERCOMX->SPI.CTRLA.bit.SWRST);
	SERCOMX->USART.CTRLA.bit.SWRST = 1;
	while (SERCOMX->USART.CTRLA.bit.SWRST || SERCOMX->USART.SYNCBUSY.bit.SWRST);

	/* Configure MSB/LSB first, 16x oversampling + arithmetic baud rate, pinouts */
	SERCOMX->USART.CTRLA.reg = (usart_cfg->lsb_first << SERCOM_USART_CTRLA_DORD_Pos)
							| SERCOM_USART_CTRLA_TXPO(usart_pinout->txpo_val)
							| SERCOM_USART_CTRLA_RXPO(usart_pinout->rxpo_val)
							| SERCOM_USART_CTRLA_SAMPR(0)
							| SERCOM_USART_CTRLA_RUNSTDBY
							| SERCOM_USART_CTRLA_MODE_USART_INT_CLK;

	/* Set baud rate (arithmetic)
	 * TODO: this only works when the SERCOM is clocked from GCLK0 (core clock) */
	SERCOMX->USART.BAUD.reg = 65536 - ((uint64_t)65536 * 16 * usart_cfg->baud) / SystemCoreClock;

	/* Enable transmitter and receiver, 8 bit character size */
	SERCOMX->USART.CTRLB.reg = SERCOM_USART_CTRLB_CHSIZE(0)
							| SERCOM_USART_CTRLB_TXEN
							| SERCOM_USART_CTRLB_RXEN;
	while (SERCOMX->USART.SYNCBUSY.bit.CTRLB);

	/* Enable SERCOM */
	SERCOMX->USART.CTRLA.bit.ENABLE = 1;
	while (SERCOMX->USART.SYNCBUSY.bit.ENABLE);
}

void usart_enable_interrupts(Sercom *SERCOMX, IRQn_Type SERCOMX_IRQn, uint8_t interrupt_mask)
{
	/* Wait for sync, then disable SERCOM */
	while (SERCOMX->USART.SYNCBUSY.bit.ENABLE);
	SERCOMX->USART.CTRLA.bit.ENABLE = 0;

	/* Set interrupt mask */
	SERCOMX->USART.INTENSET.reg = interrupt_mask;

	/* Enable interrupt handler */
	NVIC_SetPriority(SERCOMX_IRQn, 1 << __NVIC_PRIO_BITS);
	NVIC_EnableIRQ(SERCOMX_IRQn);

	// Enable SERCOM */
	SERCOMX->USART.CTRLA.bit.ENABLE = 1;
	while (SERCOMX->USART.SYNCBUSY.bit.ENABLE);
};

void usart_disable_interrupts(Sercom *SERCOMX, IRQn_Type SERCOMX_IRQn)
{
	/* Wait for sync, then disable SERCOM */
	while (SERCOMX->USART.SYNCBUSY.bit.ENABLE);
	SERCOMX->USART.CTRLA.bit.ENABLE = 0;

	/* Disable interrupt handler */
	NVIC_DisableIRQ(SERCOMX_IRQn);

	// Enable SERCOM */
	SERCOMX->USART.CTRLA.bit.ENABLE = 1;
	while (SERCOMX->USART.SYNCBUSY.bit.ENABLE);
};

void usart_write(Sercom *SERCOMX, const uint8_t *data, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
	{
		while (!SERCOMX->USART.INTFLAG.bit.DRE);
		SERCOMX->USART.DATA.reg = data[i];
	}
}

void usart_write_single(Sercom *SERCOMX, uint8_t data)
{
	while (!SERCOMX->USART.INTFLAG.bit.DRE);
	SERCOMX->USART.DATA.reg = data;
}

void usart_read(Sercom *SERCOMX, uint8_t *data, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++)
	{
		while (!SERCOMX->USART.INTFLAG.bit.RXC);
		data[i] = SERCOMX->USART.DATA.reg;
	}
}
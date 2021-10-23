/*
 * systick.c
 *
 * Created: 12/05/2021 17:01:04
 *  Author: Joshua
 */

#include "systick.h"

void systick_init()
{
	/* Disable SysTick */
	SysTick->CTRL = 0;

	/* Set period */
	SysTick->LOAD = SystemCoreClock / SYSTICK_FREQ - 1;

	/* Reset counter */
	SysTick->VAL = 0;

	/* Enable SysTick, tick interrupt, use internal (CPU) clock */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

	/* Enable interrupt with lowest priority */
	NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
	NVIC_EnableIRQ(SysTick_IRQn);
}
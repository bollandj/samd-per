
#include "usb.h"
#include "gpio.h"

static bool (*_tusb_init)(void);
static void (*_usb_int_handler)(uint8_t);

void (*usb_task)(void);

void usb_init(uint8_t gclk_48m_id, bool (*tusb_init_p)(void), void (*tud_task_p)(void), void (*tud_int_handler_p)(uint8_t))
{
	/* Register TinyUSB functions */
	_tusb_init = tusb_init_p;
	_usb_int_handler = tud_int_handler;
	usb_task = tud_task_p;

	/* Enable the APB clock for USB */
	PM->APBBMASK.reg |= PM_APBBMASK_USB;
	PM->AHBMASK.reg |= PM_AHBMASK_USB;

	/* Connect 48MHz CPU clock to USB */
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(gclk_48m_id) | GCLK_CLKCTRL_ID_USB;

	/* Wait until the clock bus is synchronized */
	while (GCLK->STATUS.bit.SYNCBUSY);

	/* Configure pins for USB */
	gpio_pin_set_output(USB_DM_PIN);
	gpio_pin_set_output(USB_DP_PIN);

	gpio_pin_set_pmux(USB_DM_PIN, USB_DM_PMUX_Val);
	gpio_pin_set_pmux(USB_DP_PIN, USB_DP_PMUX_Val);

	/* Set the priority for the USB interrupt */
	NVIC_SetPriority(USB_IRQn, 1);

	/* Initialise TinyUSB */
	_tusb_init();
}

void USB_Handler(void)
{
	_usb_int_handler(0);
}

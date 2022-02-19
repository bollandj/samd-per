
#ifndef USB_H_
#define USB_H_

#include "sam.h"
#include "tusb.h"

#define USB_DM_GROUP    GPIO_GROUP_PORTA
#define USB_DM_PIN      PIN_PA24G_USB_DM
#define USB_DM_PMUX_Val MUX_PA24G_USB_DM

#define USB_DP_GROUP    GPIO_GROUP_PORTA
#define USB_DP_PIN      PIN_PA25G_USB_DP
#define USB_DP_PMUX_Val MUX_PA25G_USB_DP

extern void (*usb_task)(void);

void usb_init(uint8_t gclk_48m_id, bool (*tusb_init_p)(void), void (*tud_task_p)(void), void (*tud_int_handler_p)(uint8_t));

#endif /* USB_H_ */
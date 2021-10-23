
#ifndef USB_H_
#define USB_H_

#include "sam.h"
#include "tusb.h"

#define USB_DM_PIN      PIN_PA24G_USB_DM
#define USB_DM_PMUX_Val MUX_PA24G_USB_DM

#define USB_DP_PIN      PIN_PA25G_USB_DP
#define USB_DP_PMUX_Val MUX_PA25G_USB_DP

void usb_init();

inline void usb_task() { tud_task(); }

#endif /* USB_H_ */
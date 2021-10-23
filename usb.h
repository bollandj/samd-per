
#ifndef USB_H_
#define USB_H_

void usb_init(uint8_t gclk_id);

inline void usb_task() { tud_task(); }

#endif /* USB_H_ */
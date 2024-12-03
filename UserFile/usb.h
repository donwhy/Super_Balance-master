#ifndef __USB_H
#define __USB_H			    
#include "app.h" 

extern usbd_core_type usb_core_dev;

void usb_init(void);
void usb_ReadWrite(void);

#endif

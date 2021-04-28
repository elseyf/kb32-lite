
#ifndef HID_H
#define HID_H

#include <stdlib.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>

#define HID_IN 0x81

extern usbd_device* usbd_dev;

void hid_setup();
void hid_write(uint8_t _modifier, uint8_t _code);

#endif
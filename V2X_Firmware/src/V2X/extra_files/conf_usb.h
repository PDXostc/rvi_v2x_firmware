/**
 * \file clock_conf.h
 *
 * \brief Initial firmware for V2X bring up
 *
 * Author: Jesse Banks (jbanks2)
 * 
 * Usage: move this file to src/config/clock_conf.h by replacing the existing file.
 */

#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "compiler.h"

#define V2X_USB_DEF

#define  USB_DEVICE_VENDOR_ID             USB_VID_ATMEL
#define  USB_DEVICE_PRODUCT_ID            USB_PID_ATMEL_ASF_SEVEN_CDC
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         2
#define  USB_DEVICE_POWER                 100 // Consumption on Vbus line (mA)
#define  USB_DEVICE_ATTR                  (USB_CONFIG_ATTR_SELF_POWERED)

#define  USB_DEVICE_MANUFACTURE_NAME      "Jaguar_Land_Rover_OSTC"
#define  USB_DEVICE_PRODUCT_NAME          "V2X"

#define  UDC_VBUS_EVENT(b_vbus_high)
#define  UDC_SOF_EVENT()                  usb_sof_action()
#define  UDC_SUSPEND_EVENT()              usb_suspend_action()
#define  UDC_RESUME_EVENT()               usb_resume_action()

#define  UDI_CDC_PORT_NB 3
#define  UDI_CDC_ENABLE_EXT(port)			usb_cdc_enable(port)
#define  UDI_CDC_DISABLE_EXT(port)			usb_cdc_disable(port)
#define  UDI_CDC_RX_NOTIFY(port)			uart_rx_notify(port)
#define  UDI_CDC_TX_EMPTY_NOTIFY(port)		//uart_tx_notify(port)
#define  UDI_CDC_SET_CODING_EXT(port,cfg)	uart_config(port,cfg)
#define  UDI_CDC_SET_DTR_EXT(port,set)		usb_cdc_set_dtr(port,set)
#define  UDI_CDC_SET_RTS_EXT(port,set)

#define  UDI_CDC_DEFAULT_RATE             9600
#define  UDI_CDC_DEFAULT_STOPBITS         CDC_STOP_BITS_1
#define  UDI_CDC_DEFAULT_PARITY           CDC_PAR_NONE
#define  UDI_CDC_DEFAULT_DATABITS         8
//! The includes of classes and other headers must be done at the end of this file to avoid compile error
#include "udi_cdc_conf.h"
#include "../V2X/V2X.h"

#endif // _CONF_USB_H_

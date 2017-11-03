/**
 * \file conf_usb.h
 *
 *  Author: Jesse Banks
 *
 * \brief USB configuration file for CDC application
 *
**/

#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "compiler.h"

/**
 * USB Device Configuration
 * @{
 */

//! Device definition
#define  USB_DEVICE_VENDOR_ID             USB_VID_ATMEL
#define  USB_DEVICE_PRODUCT_ID            USB_PID_ATMEL_MEGA_CDC_MULTI
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         2
#define  USB_DEVICE_POWER                 100 // Consumption on Vbus line (mA)
#define  USB_DEVICE_ATTR                  (USB_CONFIG_ATTR_SELF_POWERED)

//! USB Device string definitions
#define  USB_DEVICE_MANUFACTURE_NAME      "GENIVI "
#define  USB_DEVICE_PRODUCT_NAME          "V2X"

//! USB Device Callbacks definitions 
#define  UDC_VBUS_EVENT(b_vbus_high)     USB_callback_vbus_action(b_vbus_high)
#define  UDC_SOF_EVENT()                 USB_callback_sof_action()
#define  UDC_SUSPEND_EVENT()             USB_callback_suspend_action()
#define  UDC_RESUME_EVENT()              USB_callback_resume_action()

// Mandatory when USB_DEVICE_ATTR authorizes remote wakeup feature
// #define  UDC_REMOTEWAKEUP_ENABLE()        USB_callback_remotewakeup_enable()
// #define  UDC_REMOTEWAKEUP_DISABLE()       USB_callback_remotewakeup_disable()
// #define  UDC_GET_EXTRA_STRING()

//! Number of communication port used
#define  UDI_CDC_PORT_NB 3

//! Interface callback definition
#define UDI_CDC_ENABLE_EXT(port)			USB_callback_cdc_enable(port)
#define UDI_CDC_DISABLE_EXT(port)			USB_callback_cdc_disable(port)
#define  UDI_CDC_RX_NOTIFY(port)			USB_callback_rx_notify(port)
#define  UDI_CDC_TX_EMPTY_NOTIFY(port)		//USB_callback_tx_empty_notify(port)
#define  UDI_CDC_SET_CODING_EXT(port,cfg)	USB_callback_config(port,cfg)
#define  UDI_CDC_SET_DTR_EXT(port,set)		USB_callback_cdc_set_dtr(port,set)
#define  UDI_CDC_SET_RTS_EXT(port,set)		//USB_callback_cdc_set_rts(port,set)

//Transfer rate to Host is a high rate (>512000 bauds)
#define  UDI_CDC_HIGH_RATE

//! Default configuration of communication port
#define  UDI_CDC_DEFAULT_RATE             9600
#define  UDI_CDC_DEFAULT_STOPBITS         CDC_STOP_BITS_1
#define  UDI_CDC_DEFAULT_PARITY           CDC_PAR_NONE
#define  UDI_CDC_DEFAULT_DATABITS         8

//! The includes of classes and other headers must be done at the end of this file to avoid compile error
#include "udi_cdc_conf.h"
extern void USB_callback_vbus_action(bool b_vbus_high);
extern void USB_callback_sof_action(void);
extern void USB_callback_suspend_action(void);
extern void USB_callback_resume_action(void);
// extern void USB_callback_remotewakeup_enable(void);
// extern void USB_callback_remotewakeup_disable(void);

extern void USB_callback_config(uint8_t port, usb_cdc_line_coding_t * cfg);
extern Bool USB_callback_cdc_enable(uint8_t port);
extern void USB_callback_cdc_disable(uint8_t port);
//extern void USB_callback_tx_empty_notify(uint8_t port);
extern void USB_callback_rx_notify(uint8_t port);
extern void USB_callback_cdc_set_dtr(uint8_t port, bool b_enable);
//extern void USB_callback_cdc_set_rts(uint8_t port, bool b_enable);

#define V2X_USB_DEF

#endif // _CONF_USB_H_

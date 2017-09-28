/*
 * V2X_usb.h
 *
 * Created: 9/17/2017 3:39:45 PM
 *  Author: Jesse Banks
 */ 


#ifndef V2X_USB_H_
#define V2X_USB_H_

#ifndef V2X_H_
  #include "V2X.h"
#endif
/**
 * @def USB_vbus_mount
 * @brief checks the pin state and mounts the USB acordingly
 * @retval returns the v_bus state
 **/
Bool USB_vbus_mount (void);

/**
 * @def USB_callback_vbus_action
 * @brief called on vbus pin change (does not seem to be called
 * @brief replaced by other code
 **/
void USB_callback_vbus_action(bool b_vbus_high);

/**
 * @def USB_callback_sof_action
 * @brief called every frame
 **/
void USB_callback_sof_action(void);

/**
 * @def USB_callback_suspend_action
 * @brief called on suspend event
 **/
void USB_callback_suspend_action(void);

/**
 * @def USB_callback_resume_action
 * @brief called on resume event
 **/
void USB_callback_resume_action(void);

/**
 * @def USB_callback_config
 * @brief called by terminal to configure passthrough serial port
 * @param port usb_port_usage data type
 * @param cfg created by the usb software
 **/
void USB_callback_config(uint8_t port, usb_cdc_line_coding_t * cfg);

/**
 * @def USB_callback_cdc_enable
 * @brief called on port enable event
 * @param port usb_port_usage data type
 **/
Bool USB_callback_cdc_enable(uint8_t port);

/**
 * @def USB_callback_cdc_disable
 * @brief called on port disable event
 * @param port usb_port_usage data type
 **/
void USB_callback_cdc_disable(uint8_t port);

/**
 * @def USB_callback_rx_notify
 * @brief called when a char is received on any USB port
 * @param port usb_port_usage data type
 **/
void USB_callback_rx_notify (uint8_t port);

/**
 * @def USB_callback_cdc_set_dtr
 * @brief called when the data terminal mounts to receive or send data
 * @param  port usb_port_usage data type
 * @param b_enable current state
 **/
void USB_callback_cdc_set_dtr(uint8_t port, bool b_enable);

/**
 * @def USB_port_is_active
 * @brief returns the active state of mounted USB ports
 * @param port usb_port_usage data type
 * @retval current port state
 **/
Bool USB_port_is_active(uint8_t port);

/**
 * @def USB_send_string
 * @brief send a string to the USB port
 * @param port usb_port_usage data type
 * @param pointer to buffer
 **/
void USB_send_string(uint8_t port, char * buffer);

/**
 * @def USB_send_char
 * @brief send a char to the USB port
 * @param port usb_port_usage data type
 **/
void USB_send_char(uint8_t port, char value);

/**
 * @def 
 * @brief 
 * @param 
 * @retval
 **/
#endif /* V2X_USB_H_ */
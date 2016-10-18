/*
 * V2X_usb.h
 *
 * Created: 2/12/2016 10:33:34 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_USB_H_
#define V2X_USB_H_

/*! \def usb_suspend_action
 * \brief Enters the application in low power mode
 * \brief Callback called when USB host sets USB line in suspend state
 */
void usb_suspend_action(void);

/*! \def usb_resume_action
 * \brief Turn on a led to notify active mode
 * \brief Called when the USB line is resumed from the suspend state
 */
void usb_resume_action(void);

/*! \def usb_cdc_enable
 * \brief Opens the communication port
 * This is called by CDC interface when USB Host enable it.
 *
 * \retval true if cdc startup is successfully done
 */
Bool usb_cdc_enable(uint8_t port);

/*! \def usb_cdc_disable
 * \brief Closes the communication port
 * \brief This is called by CDC interface when USB Host disable it.
 */
void usb_cdc_disable(uint8_t port);

/*! \def usb_sof_action
 * \brief unused
 * \brief Called when a start of frame is received on USB line each 1ms.
 */
void usb_sof_action(void);

/*! \def usb_cdc_set_dtr
 * \brief Save new DTR state to change led behavior.
 * \breif The DTR notify that the terminal have open or close the communication port.
 * \breif sets values checked by usb_cdc_is_active
 */
void usb_cdc_set_dtr(uint8_t port, bool b_enable);

/*! \def usb_cdc_send_string
 *  \brief send a string to the usb port
 *  \param port the usb cdc port
 *  \param *buffer the message to send over the USB
 */
void usb_cdc_send_string(uint8_t port, char * buffer);

/*! \def usb_cdc_send_byte
 *	\brief send a byte to the usb port
 *  \param port the usb cdc port
 *  \param Value the byte to send over the USB
 */
void usb_cdc_send_byte(uint8_t port, int value);

/*! \def usb_cdc_is_active
 *	\brief Check if the port is actively monitored by the host
 *  \param port the usb cdc port
 */
Bool usb_cdc_is_active(uint8_t port);

#endif /* V2X_USB_H_ */
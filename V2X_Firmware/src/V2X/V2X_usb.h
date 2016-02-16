/*
 * V2X_usb.h
 *
 * Created: 2/12/2016 10:33:34 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_USB_H_
#define V2X_USB_H_

/*! \brief Enters the application in low power mode
 * Callback called when USB host sets USB line in suspend state
 */
void usb_suspend_action(void);

/*! \brief Turn on a led to notify active mode
 * Called when the USB line is resumed from the suspend state
 */
void usb_resume_action(void);

/*! \brief Opens the communication port
 * This is called by CDC interface when USB Host enable it.
 *
 * \retval true if cdc startup is successfully done
 */
bool usb_cdc_enable(uint8_t port);

/*! \brief Closes the communication port
 * This is called by CDC interface when USB Host disable it.
 */
void usb_cdc_disable(uint8_t port);

/*! \brief Manages the leds behaviors
 * Called when a start of frame is received on USB line each 1ms.
 */
void usb_sof_action(void);


/*! \brief Save new DTR state to change led behavior.
 * The DTR notify that the terminal have open or close the communication port.
 */
void usb_cdc_set_dtr(uint8_t port, bool b_enable);

#endif /* V2X_USB_H_ */
/*
 * V2X_USB.h
 *
 * Created: 2/10/2016 2:53:41 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_USB_H_
#define V2X_USB_H_


/**
 * @def usb_start
 * @brief configures and enables the SPI peripheral
 **/
void usb_start(void);
bool my_callback_cdc_enable(void);
void my_callback_cdc_disable(void);
void task(void);


#endif /* V2X_USB_H_ */
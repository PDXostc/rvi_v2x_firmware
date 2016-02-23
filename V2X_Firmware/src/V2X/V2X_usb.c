/*
 * V2X_usb.c
 *
 * Created: 2/12/2016 10:33:13 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

static volatile bool usb_cdc_enabled_bool[3] = {false, false, false};

void usb_suspend_action(void)
{
	//operation to perform when USB unplugged
}

void usb_resume_action(void)
{
	//operation to perform when USB starts
}

bool usb_cdc_enable(uint8_t port)
{
	usb_cdc_enabled_bool[port] = true;
	if (port == 0) {
		uart_open(port);	// Open communication
	}
	return true;
}

void usb_cdc_disable(uint8_t port)
{
	usb_cdc_enabled_bool[port] = false;
	// Close communication
	if (port == 0) {
		uart_close(port);	// Open communication
	}
}

void usb_sof_action(void)  //causes led 0 to flash on USB activity
{
}

void usb_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) {
		// Host terminal has open COM
		if (port == USB_CAN) {
			led_0_on();		
			//change can from direct to pass through mode
		}else if (port == USB_CMD) {
			led_1_on();
			//start Hayes interface 
		}else if (port == USB_ACL) {
			led_2_on();
			//enable ACL
			ACL_sample_on();
		}
	}else{
		// Host terminal has close COM
		if (port == USB_CAN) {
			led_0_off();
			//change can from pass through to direct mode
		}else if (port == USB_CMD) {
			led_1_off();
			//start Hayes interface
		}else if (port == USB_ACL) {
			led_2_off();
			//disable ACL
			ACL_sample_off();
		}

	}
}
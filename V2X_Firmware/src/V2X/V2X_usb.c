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
	uart_open(port);	// Open communication
	return true;
}

void usb_cdc_disable(uint8_t port)
{
	usb_cdc_enabled_bool[port] = false;
	// Close communication
	uart_close(port);
}

void usb_sof_action(void)  //causes led 0 to flash on USB activity
{
	if (!usb_cdc_enabled_bool[0] | !usb_cdc_enabled_bool[1] | !usb_cdc_enabled_bool[2]) {
		led_0_off(); 
		return;
	}
		
	int framenumber = udd_get_frame_number();
	if (0 == framenumber) {
		led_0_on();
	}
	if (1000 == framenumber) {
		led_0_off();
	}
}

void usb_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) {
		// Host terminal has open COM
		//possibly enable ELM or ACL
		//change can from direct to pass through mode
		}else{
		// Host terminal has close COM
	}
}
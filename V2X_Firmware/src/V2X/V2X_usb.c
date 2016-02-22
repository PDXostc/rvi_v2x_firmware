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
// 	if (!usb_cdc_enabled_bool[0] | !usb_cdc_enabled_bool[1] | !usb_cdc_enabled_bool[2]) {
// 		led_0_off(); 
// 		return;
// 	}
// 		
// 	int framenumber = udd_get_frame_number();
// 	if (0 == framenumber) {
// 		led_0_on();
// 	}
// 	if (1000 == framenumber) {
// 		led_0_off();
// 	}
// 	if (usb_cdc_enabled_bool[2]){
// 		uint8_t data[7];
// 		ACL_sample(data);
// 		int i = 1; //data starts with spi command
// 		while (i < 7)
// 		if (udi_cdc_multi_is_tx_ready(2)) {
// 			}else{
// 			udi_cdc_multi_putc(2, data[i]);
// 			i++;
// 		}
// 	}
}

void usb_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) {
		// Host terminal has open COM
		//possibly enable ELM or ACL
		//change can from direct to pass through mode
		if (port == 0) {
			led_0_on();
		}else if (port == 1) {
			led_1_on();
		}else if (port == 2) {
			led_2_on();
			ACL_sample_on();
		}
	}else{
		// Host terminal has close COM
		if (port == 0) {
			led_0_off();
		}else if (port == 1) {
			led_1_off();
		}else if (port == 2) {
			led_2_off();
			ACL_sample_off();
		}

	}
}
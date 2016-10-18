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

Bool usb_cdc_enable(uint8_t port)
{
	return true;
}

Bool usb_cdc_is_active(uint8_t port) {
	return usb_cdc_enabled_bool[port];
}

void usb_cdc_disable(uint8_t port)
{
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
			CAN_uart_stop();	// close direct path
			uart_open(port);	// Open USB passthrough communication
		}else if (port == USB_CMD) {
			led_1_on();
			menu_send_n_st();
			//start Hayes CMD interface 
		}else if (port == USB_ACL) {
			led_2_on();
			//enable ACL
			//ACL_sample_on();
		}
	} else {
		// Host terminal has close COM
		if (port == USB_CAN) { //change can from pass through to direct mode
			led_0_off();
			uart_close(port);	// close USB passthrough communication
			CAN_uart_start();	// open direct path
			
		}else if (port == USB_CMD) {
			led_1_off();
			//start Hayes interface
		}else if (port == USB_ACL) {
			led_2_off();
			//disable ACL
			//ACL_sample_off();
		}
	}
	usb_cdc_enabled_bool[port] = b_enable;
}

void usb_cdc_send_string(uint8_t port, char * buffer) {
	//send buffer
	int msg_l = strlen(buffer);
	int i = 0;
	while (i < msg_l) {									//buffer[i] != '\n'){
		if (!udi_cdc_multi_is_tx_ready(port)) {
			//int j = 1; //do something, Fifo full
			udi_cdc_multi_signal_overrun(port);
			//udi_cdc_ctrl_state_change(port, true, CDC_SERIAL_STATE_OVERRUN)
			}else{
			udi_cdc_multi_putc(port, buffer[i]);
			i++;
		}
	}
}

void usb_cdc_send_byte(uint8_t port, int value) {
	//send buffer
	if (!udi_cdc_multi_is_tx_ready(port)) {
		//int j = 1; //do something, Fifo full
		udi_cdc_multi_signal_overrun(port);
		//udi_cdc_ctrl_state_change(port, true, CDC_SERIAL_STATE_OVERRUN)
		}else{
		udi_cdc_multi_putc(port, value);
	}
}
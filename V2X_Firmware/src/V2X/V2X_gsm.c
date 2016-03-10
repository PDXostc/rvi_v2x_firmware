/*
 * V2X_gsm.c
 *
 * Created: 2/12/2016 11:01:08 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

char GSM_input_buffer [50] = "\0";
char GSM_output_buffer [20] = "\0";
int GSM_output_buffer_ptr = 0;
int GSM_input_buffer_ptr = 0;
int GSM_output_buffer_index = 0;
int GSM_input_buffer_index = 0;

void GSM_add_to_buffer(uint8_t buffer_select, char value) {
	switch (buffer_select) {  //output is 1
	case BUFFER_IN:
		//strcat(GSM_input_buffer, &value);					//store to buffer
		GSM_input_buffer[GSM_input_buffer_index++] = value; 
		// for test purposes relay to CMD interface
		//usb_cdc_send_byte(USB_CMD, value);
		break;
	case BUFFER_OUT:
		//strcat(GSM_output_buffer, &value);					//store to buffer
		GSM_output_buffer[GSM_output_buffer_index++] = value;
		//usb_cdc_send_byte(USB_CMD, value);
		break;
	}
}

int GSM_bytes_to_send (uint8_t buffer_select) {
	switch (buffer_select) {
	case BUFFER_IN:
		return strlen((GSM_input_buffer+GSM_input_buffer_ptr));
		break;
	case BUFFER_OUT:
		return strlen((GSM_output_buffer+GSM_output_buffer_ptr));
		break;
	}
}

char GSM_next_byte (uint8_t buffer_select) {
	switch (buffer_select) {
	case BUFFER_IN:
		return GSM_input_buffer[GSM_input_buffer_ptr++];
		break;
	case BUFFER_OUT:
		return GSM_output_buffer[GSM_output_buffer_ptr++];
		break;
	}
}

void GSM_purge_buffer(uint8_t buffer_select) {
	switch (buffer_select) {
	case BUFFER_IN:
		clear_buffer(GSM_input_buffer);
		GSM_input_buffer_ptr = 0;
		GSM_input_buffer_index = 0;
		break;
	case BUFFER_OUT:
		clear_buffer(GSM_output_buffer);
		GSM_output_buffer_ptr = 0;
		GSM_output_buffer_index = 0;
		break;
	}
}

void GSM_process_buffer (uint8_t buffer_select) {
	switch (buffer_select) {
		case BUFFER_IN:
			usb_tx_string_P(PSTR("GSM>>>:"));
			usb_cdc_send_string(USB_CMD, GSM_input_buffer);
			usb_tx_string_P(PSTR("\r>"));
			GSM_purge_buffer(BUFFER_IN);
			break;
		case BUFFER_OUT:
			if (GSM_bytes_to_send(BUFFER_OUT)) {
				GSM_set_tx_int(); //set to continue sending buffer
				//USART_SIM.DATA = GSM_next_byte(BUFFER_OUT);
			}
			break;
	}
}


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
		GSM_input_buffer[GSM_input_buffer_index++] = value; 
		break;
	case BUFFER_OUT:
		GSM_output_buffer[GSM_output_buffer_index++] = value;
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
			}
			break;
	}
}

void GSM_usart_init (void) {
	sysclk_enable_module(USART_SIM_PORT_SYSCLK, USART_SIM_SYSCLK);
	USART_SIM.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_SIM_CHAR_LENGTH | USART_SIM_PARITY | USART_SIM_STOP_BIT;
	USART_SIM.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	uint16_t b_sel = (uint16_t) (((((((uint32_t) sysclk_get_cpu_hz()) << 1) / ((uint32_t) USART_SIM_BAUDRATE * 8)) + 1) >> 1) - 1);
	USART_SIM.BAUDCTRLA = b_sel & 0xFF;
	USART_SIM.BAUDCTRLB = b_sel >> 8;
	USART_SIM_PORT.DIRSET = USART_PORT_PIN_TX; // TX as output.
	USART_SIM_PORT.DIRCLR = USART_PORT_PIN_RX; // RX as input.
	GSM_clear_tx_int();
	GSM_add_to_buffer(BUFFER_IN, '\n');
	GSM_purge_buffer(BUFFER_IN);
	GSM_add_to_buffer(BUFFER_OUT, '\n');
	GSM_process_buffer(BUFFER_OUT);
	GSM_purge_buffer(BUFFER_OUT);
}

void GSM_set_tx_int(void) {
	USART_SIM.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_HI_gc;
}

void GSM_clear_tx_int(void) {
	USART_SIM.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_OFF_gc;
}

ISR(USART_SIM_RX_Vect)
{	// Transfer UART RX fifo to buffer
	char value = USART_SIM.DATA;
	if (value == '\n') {
		GSM_process_buffer(BUFFER_IN);
		} else {
		GSM_add_to_buffer(BUFFER_IN, value);
	}
}

ISR(USART_SIM_DRE_Vect)
{
	if (GSM_bytes_to_send(BUFFER_OUT)) {
		USART_SIM.DATA = GSM_next_byte(BUFFER_OUT);
		} else {
		GSM_purge_buffer(BUFFER_OUT);
		GSM_clear_tx_int();
	}
}
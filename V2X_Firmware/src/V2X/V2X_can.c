/*
 * V2X_can.c
 *
 * Created: 3/10/2016 12:25:37 PM
 *  Author: jbanks2
 */ 

#include "V2X.h"

char CAN_input_buffer[30];
char CAN_output_buffer[30];
int CAN_output_buffer_ptr = 0;
int CAN_input_buffer_ptr = 0;
int CAN_output_buffer_index = 0;
int CAN_input_buffer_index = 0;

void CAN_add_to_buffer(uint8_t buffer_select, char value) {
	switch (buffer_select) {  //output is 1
		case BUFFER_IN:
		//udi_cdc_multi_putc(USB_CAN, value);
		CAN_input_buffer[CAN_input_buffer_index++] = value;
		break;
		case BUFFER_OUT:
		//udi_cdc_multi_putc(USB_CAN, value);
		CAN_output_buffer[CAN_output_buffer_index++] = value;
		break;
	}
}

int CAN_bytes_to_send (uint8_t buffer_select) {
	switch (buffer_select) {
		case BUFFER_IN:
		return strlen((CAN_input_buffer+CAN_input_buffer_ptr));
		break;
		case BUFFER_OUT:
		return strlen((CAN_output_buffer+CAN_output_buffer_ptr));
		break;
	}
}

char CAN_next_byte (uint8_t buffer_select) {
	switch (buffer_select) {
		case BUFFER_IN:
		return CAN_input_buffer[CAN_input_buffer_ptr++];
		break;
		case BUFFER_OUT:
		return CAN_output_buffer[CAN_output_buffer_ptr++];
		break;
	}
}

void CAN_purge_buffer(uint8_t buffer_select) {
	switch (buffer_select) {
	case BUFFER_IN:
		clear_buffer(CAN_input_buffer);
		CAN_input_buffer_ptr = 0;
		CAN_input_buffer_index = 0;
		break;
	case BUFFER_OUT:
		clear_buffer(CAN_output_buffer);
		CAN_output_buffer_ptr = 0;
		CAN_output_buffer_index = 0;
		break;
	}
}
void CAN_uart_start (void) {
	uart_open(USB_CAN);
	uint16_t b_sel = (uint16_t) (((((((uint32_t) sysclk_get_cpu_hz()) << 1) / ((uint32_t) USART_BAUDRATE * 8)) + 1) >> 1) - 1);
	USART.BAUDCTRLA = b_sel & 0xFF;
	USART.BAUDCTRLB = b_sel >> 8;
	USART_SIM.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	USART.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHAR_LENGTH | USART_PARITY | USART_STOP_BIT;
	CAN_clear_tx_int();
	CAN_add_to_buffer(BUFFER_IN, '\n');
	CAN_purge_buffer(BUFFER_IN);
	CAN_add_to_buffer(BUFFER_OUT, '\n');
	CAN_process_buffer(BUFFER_OUT);
	CAN_purge_buffer(BUFFER_OUT);
}

void CAN_uart_stop (void) {
	uart_close(false);
}

void CAN_set_tx_int(void) {
	USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_HI_gc;
}

void CAN_clear_tx_int(void) {
	USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_OFF_gc;
}

void CAN_process_buffer (uint8_t buffer_select) {
	switch (buffer_select) {
	case BUFFER_IN:
		usb_tx_string_P(PSTR("CAN>>>:"));
		usb_cdc_send_string(USB_CMD, CAN_input_buffer);
		usb_tx_string_P(PSTR("\r>"));
		CAN_purge_buffer(BUFFER_IN);
		break;
	case BUFFER_OUT:
		if (CAN_bytes_to_send(BUFFER_OUT)) {
			CAN_set_tx_int(); //set to continue sending buffer
		}
		break;
	}
}
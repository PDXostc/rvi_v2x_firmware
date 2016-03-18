/*
 * V2X_can.c
 *
 * Created: 3/10/2016 12:25:37 PM
 *  Author: jbanks2
 */ 

#include "V2X.h"

volatile buff CAN;

 void CAN_add_to_buffer(uint8_t buffer_select, char value) {
	CTL_add_to_buffer(&CAN, buffer_select, value);
 }
 
 int CAN_bytes_to_send (uint8_t buffer_select) {
	return CTL_bytes_to_send(&CAN, buffer_select);
 }
 
 char CAN_next_byte (uint8_t buffer_select) {
	return CTL_next_byte(&CAN, buffer_select);
 }
 
 void CAN_purge_buffer(uint8_t buffer_select) {
	CTL_purge_buffer(&CAN, buffer_select);
 }

void CAN_mark_for_processing (Bool in_out) {
	CTL_mark_for_processing(&CAN, in_out);
}

void CAN_add_string_to_buffer(Bool in_out, char * to_add) {
	CTL_add_string_to_buffer(&CAN, in_out, to_add);
}

void CAN_uart_start (void) {
	uart_open(USB_CAN);
	uint16_t b_sel = (uint16_t) (((((((uint32_t) sysclk_get_cpu_hz()) << 1) / ((uint32_t) USART_BAUDRATE * 8)) + 1) >> 1) - 1);
	USART.BAUDCTRLA = b_sel & 0xFF;
	USART.BAUDCTRLB = b_sel >> 8;
	USART_SIM.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	USART.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHAR_LENGTH | USART_PARITY | USART_STOP_BIT;
	CAN_clear_tx_int();

	CAN.in_store_a = CAN.in_store_b = CAN.out_store_a = CAN.out_store_b = 0;
	CAN.in_proc_a = CAN.in_proc_b = CAN.out_proc_a = CAN.out_proc_b = 0;
	CAN.active_in = CAN.active_out = BUFFER_A;	

	CTL_add_to_buffer(&CAN, BUFFER_IN, '\n'); //A
	CTL_mark_for_processing(&CAN, BUFFER_IN);
	CTL_purge_buffer(&CAN, BUFFER_IN);
	CTL_add_to_buffer(&CAN, BUFFER_IN, '\n'); //B
	CTL_mark_for_processing(&CAN, BUFFER_IN);
	CTL_purge_buffer(&CAN, BUFFER_IN);

	CTL_add_to_buffer(&CAN, BUFFER_OUT, '\n');
	CTL_mark_for_processing(&CAN, BUFFER_OUT);
	CTL_purge_buffer(&CAN, BUFFER_OUT);
	CTL_add_to_buffer(&CAN, BUFFER_OUT, '\n');
	CTL_mark_for_processing(&CAN, BUFFER_OUT);
	CTL_purge_buffer(&CAN, BUFFER_OUT);
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

void CAN_process_buffer (void) {
	// if there is something received
	if (CTL_bytes_to_send(&CAN, BUFFER_IN)) {
		usb_tx_string_P(PSTR("CAN>>>:"));
		usb_cdc_send_string(USB_CMD, CTL_ptr_to_proc_buffer(&CAN, BUFFER_IN));
		usb_tx_string_P(PSTR("\r>"));
		CTL_purge_buffer(&CAN, BUFFER_IN);
	}
	if (CTL_bytes_to_send(&CAN, BUFFER_OUT)) {
		CAN_set_tx_int(); //set to send buffer
	} 

}

void CAN_power_off (void) {
	power_control_turn_off((1<<ENABLE_CAN_RESET));
	power_control_push();
};
	
void CAN_power_on (void) {
	power_control_turn_on((1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP));
	power_control_push();
};

void CAN_restart (void) {
	CAN_power_off();
	delay_ms(10);
	CAN_power_on();
}
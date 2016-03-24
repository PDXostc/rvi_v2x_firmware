/*
 * V2X_can.c
 *
 * Created: 3/10/2016 12:25:37 PM
 *  Author: jbanks2
 */ 

#include "V2X.h"

volatile buff CAN;
uint8_t CAN_sequence_state = CAN_state_idle;
uint8_t CAN_subsequence_state = CAN_subssequence_1;

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

	CAN.input_proc_flag = CAN.input_proc_index = CAN.input_index = 0;
	CAN.output_proc_active_flag = CAN.output_proc_index = 0;
	CTL_add_to_buffer(&CAN, BUFFER_IN, '\r'); //put something in buffer so pointers are different
}

void CAN_uart_stop (void) {
	uart_close(USB_CAN);
}

void CAN_set_tx_int(void) {
	USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_HI_gc;
}

void CAN_clear_tx_int(void) {
	USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_OFF_gc;
}

void CAN_process_buffer (void) {
	if (CAN.output_proc_active_flag) {
		if (CAN.output_proc_loaded) { //output buffer is ready to send
			CAN_set_tx_int();		//set ISR flag
			} else {
			CTL_purge_buffer(&CAN, BUFFER_OUT);
		}
	}
	while (CAN.input_proc_flag) {
		CTL_copy_to_proc(&CAN); //copy string from buffer
		if (CAN.input_proc_loaded) { //process string
			menu_send_CAN();
			usb_cdc_send_string(USB_CMD, CAN.input_proc_buf);
			menu_send_n_st();
			CAN_control(CAN.input_proc_buf);
			CAN.input_proc_loaded = false;		//input proc buffer has been handled
		}
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

void CAN_control (char * responce_buffer) {
	switch (CAN_sequence_state) {
	case CAN_state_idle:
	case CAN_state_check:
	case CAN_state_start:
	default:
		CAN_sequence_state = CAN_state_idle;
		break;
	}
}

void CAN_control_fail (void) {
	CAN_subsequence_state = CAN_subssequence_FAIL;
}
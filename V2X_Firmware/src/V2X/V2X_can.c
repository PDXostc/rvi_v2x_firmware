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
Bool CAN_in_command = false;
Bool CAN_snoop = false;

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

void CAN_mark_as_sent(void) {
	CAN.output_proc_loaded = false; //mark string as sent
}

void CAN_uart_start (void) {
		uart_open(USB_CAN);
		uint16_t b_sel = (uint16_t) (((((((uint32_t) sysclk_get_cpu_hz()) << 1) / ((uint32_t) USART_BAUDRATE * 8)) + 1) >> 1) - 1);
		USART.BAUDCTRLA = b_sel & 0xFF;
		USART.BAUDCTRLB = b_sel >> 8;
		USART.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHAR_LENGTH | USART_PARITY | USART_STOP_BIT;
		
		CAN.input_proc_flag = CAN.input_proc_index = CAN.input_index = 0;
		CAN.output_proc_active_flag = CAN.output_proc_index = 0;
		CTL_add_to_buffer(&CAN, BUFFER_IN, '\r'); //put something in buffer so pointers are different
}

void CAN_uart_stop (void) {
	uart_close(USB_CAN);
}

Bool CAN_is_controlled(void) {
	return CAN_in_command;
}

void CAN_start_snoop (void) {
	job_set_timeout(SYS_CAN_CTL, 4); //listen for responses for 4 seconds
	CAN_snoop = true;
}

void CAN_stop_snoop (void) {
	job_clear_timeout(SYS_CAN_CTL);
	CAN_snoop = false;
}

Bool CAN_is_snooping(void) {
	return CAN_snoop;
}

void CAN_set_tx_int(void) {
	USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_HI_gc;
}

void CAN_clear_tx_int(void) {
	USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_OFF_gc;
}

void CAN_new_data (uint8_t value) {
	CAN_add_to_buffer(BUFFER_IN, value);
	if (value == '>' || value == '\r' || value == 0x07) {
		CAN_mark_for_processing(BUFFER_IN);
	}
}

void CAN_send_data (void) {	
	if (CAN_bytes_to_send(BUFFER_OUT)) {
		USART.DATA = CAN_next_byte(BUFFER_OUT);
		} else {
		CAN_clear_tx_int();
		CAN_mark_as_sent();
	}
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

void CAN_control (char * responce_buffer) {
	switch (CAN_sequence_state) {
	case CAN_state_idle:
		CAN_in_command = false;
		break;
	case CAN_state_power:
		CAN_control_init(responce_buffer);
		break;
	case CAN_state_start:
	default:
		CAN_sequence_state = CAN_state_idle;
		break;
	}
}

void CAN_control_fail (void) {
	CAN_subsequence_state = CAN_subssequence_FAIL;
}

void CAN_elm_init (void) {
	if (CAN_sequence_state == CAN_state_idle) {
		CAN_sequence_state = CAN_state_power;
		CAN_subsequence_state = CAN_subssequence_1; //move to response state
		CAN_in_command = true; //make sure responces come back to comand processor
		char hold[2] = "\0";
		CAN_control (hold);
	}
}

void CAN_control_init (char * responce_buffer){
	switch (CAN_subsequence_state) {
	case CAN_subssequence_1:  //check module power
		if (PWR_query((1<<ENABLE_5V0))) { //is the module power on?
			CAN_subsequence_state = CAN_subssequence_2;
			CAN_control(responce_buffer);
			} else { //if not power it up
			usb_tx_string_P(PSTR(">CTL>>>:Power up CAN\r"));  //does not need end of string, exits through menu
			PWR_can_start();
			CAN_subsequence_state = CAN_subssequence_4;
			job_set_timeout(SYS_CAN, 4); //give elm module 3 seconds to start
		}
		break;
	case CAN_subssequence_2: //is chip enabled?
		if (PWR_query((1<<ENABLE_CAN_RESET)) == false) { //no enable it
			PWR_can_stop();
			PWR_can_start();
			job_set_timeout(SYS_CAN, 4); //give elm module 3 seconds to start
			CAN_subsequence_state = CAN_subssequence_1; //should wake blurt
		} else {
			CAN_subsequence_state = CAN_subssequence_3; //needs ati to check responce
			CAN_control(responce_buffer);
		}
		break;
	case CAN_subssequence_3: //push ATI to can
		CTL_add_string_to_buffer_P(&CAN, BUFFER_OUT, PSTR("ATI\r")); //compose message
		CTL_mark_for_processing(&CAN, BUFFER_OUT); //send it
		CAN_subsequence_state = CAN_subssequence_4; //move to response state
		job_set_timeout(SYS_CAN, 2);
		break;
	case CAN_subssequence_4: //Module responce
		if (strcmp_P(responce_buffer, PSTR("LV RESET")) == 0) {
			CAN_subsequence_state = CAN_subssequence_3;
			menu_send_CTL();
			usb_tx_string_P(PSTR("CAN Powered\r>"));
			CAN_control(responce_buffer);
		}
		if (strcmp_P(responce_buffer, PSTR("ELM327 v1.3a")) == 0) {
			menu_send_CTL();
			usb_tx_string_P(PSTR("CAN Responding\r>"));
			CAN_sequence_state = CAN_state_idle;
			job_clear_timeout(SYS_CAN);
		}
		job_check_fail(SYS_CAN);
		break;
	case CAN_subssequence_FAIL:
		default:
		CAN_sequence_state = CAN_state_idle;
		job_clear_timeout(SYS_CAN);
		menu_send_CTL();
		usb_tx_string_P(PSTR("CAN start fail\r>"));
		break;
	}
}
/*
 * V2X_can.c
 *
 * Created: 3/10/2016 12:25:37 PM
 *  Author: jbanks2
 */ 

#include "V2X.h"

volatile buff CAN;
uint8_t CAN_sequence_state = CAN_state_idle;
uint8_t CAN_init_subsequence_state = CAN_init_subsequence_1;
uint8_t CAN_ee_subsequence_state = CAN_ee_subsequence_1;
uint8_t CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_1;
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
            //CSC_can_input_buffer(CAN.input_proc_buf);
			CAN.input_proc_loaded = false;		//input proc buffer has been handled
		}
	}
}

void CAN_control (char * response_buffer) {
	switch (CAN_sequence_state) {
	case CAN_state_idle:
		CAN_in_command = false;
		break;
	case CAN_state_power:
		CAN_control_init(response_buffer);
		break;
	case CAN_state_EE:
		CAN_ee_sequence(response_buffer);
		break;
    case CAN_state_read_voltage:
        CAN_read_voltage_sequence(response_buffer);
        break;
	default:
		CAN_sequence_state = CAN_state_idle;
		break;
	}
}

void CAN_control_init_fail(void) {
	CAN_init_subsequence_state = CAN_init_subsequence_FAIL;
}

void CAN_elm_init (void) {
	if (CAN_sequence_state == CAN_state_idle) {
		CAN_sequence_state = CAN_state_power;
		CAN_init_subsequence_state = CAN_init_subsequence_1; //move to response state
		CAN_in_command = true; //make sure responses come back to command processor
		char hold[2] = "\0";
		CAN_control (hold);
	} else {
        CAN_init_subsequence_state = CAN_init_subsequence_FAIL;
    }
}

void CAN_control_init (char * response_buffer){
	switch (CAN_init_subsequence_state) {
	case CAN_init_subsequence_1:  //check module power
		if (PWR_query((1<<ENABLE_5V0))) { //is the module power on?
			CAN_init_subsequence_state = CAN_init_subsequence_2;
			CAN_control(response_buffer);
        } else { //if not power it up
			usb_tx_string_P(PSTR(">CTL>>>:Power up CAN\r\n"));  //does not need end of string, exits through menu
			PWR_can_start();
			CAN_init_subsequence_state = CAN_init_subsequence_4;
			job_set_timeout(SYS_CAN, 4); //give elm module 3 seconds to start
		}
		break;
	case CAN_init_subsequence_2: //is chip enabled?
		if (PWR_query((1<<ENABLE_CAN_RESET)) == false) { //no enable it
			PWR_can_stop();
			PWR_can_start();
			job_set_timeout(SYS_CAN, 4); //give elm module 3 seconds to start
			CAN_init_subsequence_state = CAN_init_subsequence_1; //should wake blurt
		} else {
			CAN_init_subsequence_state = CAN_init_subsequence_3; //needs ati to check response
			CAN_control(response_buffer);
		}
		break;
	case CAN_init_subsequence_3: //push ATI to can
		CTL_add_string_to_buffer_P(&CAN, BUFFER_OUT, PSTR("ATI\r")); //compose message
		CTL_mark_for_processing(&CAN, BUFFER_OUT); //send it
		CAN_init_subsequence_state = CAN_init_subsequence_4; //move to response state
		job_set_timeout(SYS_CAN, 2);
		break;
	case CAN_init_subsequence_4: //Module response
		if (strcmp_P(response_buffer, PSTR("LV RESET")) == 0) { // if high-power, probably tell us this
			CAN_init_subsequence_state = CAN_init_subsequence_3;
			menu_send_CTL();
			usb_tx_string_P(PSTR("CAN Powered\r\n>"));
			CAN_control(response_buffer);
		} else if (strcmp_P(response_buffer, PSTR("ELM327 v1.3a")) == 0) { // if booting-up probably be here
			menu_send_CTL();
			usb_tx_string_P(PSTR("CAN Responding\r\n>"));
			CAN_sequence_state = CAN_state_idle; // Lilli note - Maybe change to 'initialized'? do more here...
            CAN_init_subsequence_state = CAN_init_subsequence_COMPLETE;
			CAN_in_command = false;
			job_clear_timeout(SYS_CAN);
		} else {
			job_check_fail(SYS_CAN);
		}
		break;
	case CAN_init_subsequence_FAIL:
		default:
		CAN_sequence_state = CAN_state_idle;
		CAN_in_command = false;
		job_clear_timeout(SYS_CAN);
		menu_send_CTL();
		usb_tx_string_P(PSTR("CAN start fail\r\n>"));
		break;
	}
}

void CAN_EE_start (void) {
	if (CAN_sequence_state == CAN_state_idle) {
		CAN_sequence_state = CAN_state_EE;
		CAN_ee_subsequence_state = CAN_ee_subsequence_1; //move to response state
		CAN_in_command = true; //make sure responses come back to command processor
		char hold[2] = "\0";
		CAN_control (hold);
    } else {
        CAN_ee_subsequence_state = CAN_ee_subsequence_FAIL;
    }
}

Bool CAN_find_message (char * buffer, uint8_t index) {
	uint32_t start_ptr = buffer; //start at beginning of buffer
	int length = strlen(buffer);	
	int i = 0;	//index used throughout module
	//move through buffer the number of commas in step
	while (index != 0) {
		start_ptr = strchr(start_ptr, ',') + 1; //look for comma wanted
		index--;		
	}
	int start_index = start_ptr - (uint32_t) buffer; 
	uint32_t stop_ptr = strchr(start_ptr, ','); //find next comma
	if (stop_ptr == 0) {
		return false;  //buffer has no more messages
	}
	int span = stop_ptr - start_ptr;  //save span of string
	for (i = 0; i < span; i++) {  
		buffer[i] = buffer[i + start_index]; //copy wanted section to front
	}
	buffer[i++] = '\r';
	while (i < length) {
		buffer[i++] = '\0'; //fill end string with garbage
	}
	return true; //message was successfully parsed
}

void CAN_ee_sequence (char * response_buffer) {
	char buffer[EE_CAN_ARRAY_SIZE+1];
	uint8_t found;
	if (CAN_ee_subsequence_state == CAN_ee_subsequence_FAIL) {
			CAN_sequence_state = CAN_state_idle;
			job_clear_timeout(SYS_CAN);
			menu_send_CAN();
			usb_tx_string_P(PSTR("ERROR: EEPROM sequence fail\r\n>"));
	} else if (CAN_ee_subsequence_state != CAN_ee_subsequence_1) { //not the first state, response_buffer should have response
		if (strcmp_P(response_buffer, PSTR("OK")) == 0) { //if response was OK
			eeprom_read_CAN_string(buffer); //get copy of the entire string
			found = CAN_find_message(buffer, CAN_ee_subsequence_state++);
			if (found) {
				CTL_add_string_to_buffer(&CAN, BUFFER_OUT, buffer);
				CTL_mark_for_processing(&CAN, BUFFER_OUT);
				job_set_timeout(SYS_CAN, 2); //give ELM module 2 seconds to respond
			} else {
				CAN_sequence_state = CAN_state_idle;
				job_clear_timeout(SYS_CAN);
				menu_send_CAN();
				usb_tx_string_P(PSTR("EEPROM sequence complete\r\n>"));
			}
		} else { //the response was bad
			CAN_ee_subsequence_state == CAN_ee_subsequence_FAIL;
			job_set_timeout(SYS_CAN, 1);  //end quickly
		}
	} else { //is the first command, just send a message
		eeprom_read_CAN_string(buffer); //get copy of the entire string
		if (found = CAN_find_message(buffer, CAN_ee_subsequence_state++)) {//CAN_subsequence_state);
			CTL_add_string_to_buffer(&CAN, BUFFER_OUT, buffer);
			CTL_mark_for_processing(&CAN, BUFFER_OUT);
			job_set_timeout(SYS_CAN, 2); //give ELM module 2 seconds to respond
		} else { //the buffer was bad
			CAN_ee_subsequence_state == CAN_ee_subsequence_FAIL;
			job_set_timeout(SYS_CAN, 1);  //end quickly
		}
	}
}

void CAN_read_voltage_start() {
    if (CAN_sequence_state == CAN_state_idle) {
        CAN_sequence_state = CAN_state_read_voltage;
        CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_1;
        CAN_in_command = true; //make sure responses come back to command processor
        char hold[2] = "\0"; // ??
        CAN_control (hold);
    } else {
        CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_FAIL;
    }
}

//void CAN_read_voltage_stop() {
//    CAN_sequence_state = CAN_state_idle;
//}

void CAN_read_voltage_sequence (char * response_buffer) {
 
}

uint8_t CAN_get_sequence_state() {
    return CAN_sequence_state;
}

uint8_t CAN_get_init_subsequence_state() {
    return CAN_init_subsequence_state;
}


/*
 * V2X_can.c
 *
 * Created: 3/10/2016 12:25:37 PM
 *  Author: Jesse Banks
 */

#include <errno.h>
#include "V2X.h"

volatile buff CAN;
uint8_t CAN_sequence_state = CAN_state_idle;
uint8_t CAN_init_subsequence_state = CAN_init_subsequence_1;
uint8_t CAN_ee_subsequence_state = CAN_ee_subsequence_1;
uint8_t CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_1;
uint8_t CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_1;
Bool CAN_in_CTL = false;
Bool CAN_snoop = false;

double CAN_last_read_voltage = 0;
Bool CAN_last_did_hear_chatter = false;
uint8_t CAN_chatter_count = 0;

#define READ_VOLTAGE_DIODE_DROP  (0.6)
#define CAN_HEARD_CHATTER_CUTOFF 3


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

void CAN_mark_for_processing (Bool buffer_select) {
	CTL_mark_for_processing(&CAN, buffer_select);
}

void CAN_add_string_to_buffer(Bool buffer_select, char * to_add) {
	CTL_add_string_to_buffer(&CAN, buffer_select, to_add);
}

void CAN_mark_as_sent(void) {
	CAN.output_proc_loaded = false; //mark string as sent
}

void CAN_uart_start (void) {
	usart_rs232_options_t usart_cfg = {
		.baudrate = CAN_BAUDRATE,
		.charlength = CAN_CHAR_LENGTH,
		.paritytype = CAN_PARITY,
		.stopbits = CAN_STOP_BIT 
	};
	
	//start UART
	sysclk_enable_module(CAN_PORT_SYSCLK, CAN_SYSCLK);
	usart_init_rs232(CAN_UART, &usart_cfg);  //returns true if successfull at calculating the baud rate
	usart_set_rx_interrupt_level(CAN_UART, USART_INT_LVL_HI);
	usart_set_tx_interrupt_level(CAN_UART, USART_INT_LVL_OFF);
	
	//initialize buffers
	CAN.input_proc_flag = CAN.input_proc_index = CAN.input_index = 0;
	CAN.output_proc_active_flag = CAN.output_proc_index = 0;
	CTL_add_to_buffer(&CAN, BUFFER_IN, '\n'); //put something in buffer so pointers are different
	CTL_mark_for_processing(&CAN, BUFFER_IN);
}

void CAN_uart_stop (void) 
{
	usart_rx_disable(CAN_UART);
	usart_tx_disable(CAN_UART);
}

Bool CAN_is_controlled(void) 
{
	return CAN_in_CTL;
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
	usart_set_tx_interrupt_level(CAN_UART, USART_INT_LVL_HI);
}

void CAN_clear_tx_int(void) {
	usart_set_tx_interrupt_level(CAN_UART, USART_INT_LVL_OFF);
}

void CAN_new_data (uint8_t value) {
	CTL_add_to_buffer(&CAN, BUFFER_IN, value);
	if ( value == '>' || value == '\r' ) {
		CAN_mark_for_processing(BUFFER_IN);  //called from inside ISR, mark flag and exit
	}
}

void  CAN_send_data (void) {	
	if (CAN_bytes_to_send(BUFFER_OUT)) {
		usart_putchar(CAN_UART, CAN_next_byte(BUFFER_OUT) );
	} else {
		CAN_mark_as_sent();
 		CAN_clear_tx_int();
 	}
}

void CAN_process_buffer (void) {
	if (CAN.output_proc_active_flag) {
		if (CAN.output_proc_loaded) { //output buffer is ready to send
			CAN_set_tx_int();		//set ISR flag
			if (CAN.output_proc_index == 0) {//new command
				CAN_send_data();
			}
		} else {
			CTL_purge_buffer(&CAN, BUFFER_OUT);
		}
	}
	while (CAN.input_proc_flag) {
		CTL_copy_to_proc(&CAN); //copy string from buffer
		if (CAN.input_proc_loaded) { //process string
			menu_send_CAN();
 			USB_send_string(USB_CMD, CAN.input_proc_buf);
 			menu_send_n_st();
			CAN_control(CAN.input_proc_buf);
			CAN.input_proc_loaded = false;		//input proc buffer has been handled
		}
	}
}

void CAN_control (char * response_buffer) {
	switch (CAN_sequence_state) {
	case CAN_state_idle:
		CAN_in_CTL = false;
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
    case CAN_state_hear_chatter:
        CAN_hear_chatter_sequence(response_buffer);
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
		USB_tx_string_P(PSTR("CAN>:Init begin\r\n"));
		CAN_sequence_state = CAN_state_power;
		CAN_init_subsequence_state = CAN_init_subsequence_1; //move to response state
		CAN_in_CTL = true; //make sure responses come back to command processor
		char hold[2] = "\0";
		CAN_control (hold);
	} else {
		USB_tx_string_P(PSTR("CAN:> Init failed to start\r\n"));
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
//			USB_tx_string_P(PSTR(">CTL>>>:Power up CAN\r\n"));  //does not need end of string, exits through menu
			PWR_4_start();
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
			USB_tx_string_P(PSTR("CAN Powered\r\n>"));
			CAN_control(response_buffer);
		} else if (strcmp_P(response_buffer, PSTR("ELM327 v1.3a")) == 0) { // if booting-up probably be here
			menu_send_CTL();
			USB_tx_string_P(PSTR("CAN Responding\r\n>"));
			CAN_sequence_state = CAN_state_idle;
            CAN_init_subsequence_state = CAN_init_subsequence_COMPLETE;
			CAN_in_CTL = false;
			job_clear_timeout(SYS_CAN);
		} else {
			job_check_fail(SYS_CAN);
		}
		break;
	case CAN_init_subsequence_FAIL:
		default:
 		CAN_sequence_state = CAN_state_idle;
// 		CAN_sequence_state = CAN_state_power;
// 		CAN_init_subsequence_state = CAN_init_subsequence_1;
		PWR_can_stop();
		//CAN_in_CTL = false;
		job_set_timeout(SYS_CAN, 2);
		menu_send_CTL();
		USB_tx_string_P(PSTR("CAN init fail\r\n>"));
		break;
	}
}

void CAN_EE_start (void) {
	if (CAN_sequence_state == CAN_state_idle) {
		CAN_sequence_state = CAN_state_EE;
		CAN_ee_subsequence_state = CAN_ee_subsequence_1; //move to response state
		CAN_in_CTL = true; //make sure responses come back to command processor
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
			USB_tx_string_P(PSTR("ERROR: EEPROM sequence fail"));
			menu_send_n_st();
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
				CAN_ee_subsequence_state = CAN_ee_subsequence_COMPLETE;
				job_clear_timeout(SYS_CAN);
				menu_send_CAN();
				USB_tx_string_P(PSTR("EEPROM sequence complete"));
				menu_send_n_st();
			}
		} else { //the response was bad
			CAN_ee_subsequence_state = CAN_ee_subsequence_FAIL;
			job_set_timeout(SYS_CAN, 1);  //end quickly
		}
	} else { //is the first command, just send a message
		eeprom_read_CAN_string(buffer); //get copy of the entire string
		if ( ( found = CAN_find_message(buffer, CAN_ee_subsequence_state++) ) ) {//CAN_subsequence_state);
			CTL_add_string_to_buffer(&CAN, BUFFER_OUT, buffer);
			CTL_mark_for_processing(&CAN, BUFFER_OUT);
			job_set_timeout(SYS_CAN, 2); //give ELM module 2 seconds to respond
		} else { //the buffer was bad
			CAN_ee_subsequence_state = CAN_ee_subsequence_FAIL;
			job_set_timeout(SYS_CAN, 1);  //end quickly
		}
	}
}

void CAN_read_voltage_start(void) {
    if (CAN_sequence_state == CAN_state_idle) {
        CAN_sequence_state = CAN_state_read_voltage;
        CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_1;
        CAN_in_CTL = true; //make sure responses come back to command processor
        char hold[2] = "\0"; // ??
        CAN_control (hold);
    } else {
        CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_FAIL;
    }
}

/**
 * Parses the buffer for a valid voltage reading
 * @param buffer
 * @return the voltage reading, or -1 if there's an error
 */
double CAN_parse_voltage_regex(char * buffer);
double CAN_parse_voltage_regex(char * buffer) {

    /* If the value starts with '0', forget the decimal and return 0, as the voltage may as well be 0.
     * Doing this because strtod returns 0.0 if there's an error. Not sure how else to test if actually 0. */

    char* pEnd;
    double voltageVal;

    errno = 0;
    voltageVal = strtod(buffer, &pEnd);

    if (errno == ERANGE)
        return -1;

    if (!voltageVal)
        return -1;

    if (pEnd[0] != 'V')
        return -1;

    return voltageVal;
}

void CAN_read_voltage_sequence (char * response_buffer) {
    switch (CAN_read_voltage_subsequence_state) {
        case CAN_read_voltage_subsequence_1:
            CTL_add_string_to_buffer_P(&CAN, BUFFER_OUT, PSTR("ATRV\r")); //compose message
            CTL_mark_for_processing(&CAN, BUFFER_OUT); //send it

            CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_2; //move to response state
            job_set_timeout(SYS_CAN, 2);

            break;

        case CAN_read_voltage_subsequence_2: //Module response
            if (strcmp_P(response_buffer, PSTR("ATRV")) == 0) {
                CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_3;
                job_set_timeout(SYS_CAN, 1);

            } else {
                CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_FAIL;
                CAN_control(response_buffer);

            }

            break;

        case CAN_read_voltage_subsequence_3:
            if (CAN_parse_voltage_regex(response_buffer) != -1) {

                CAN_last_read_voltage = CAN_parse_voltage_regex(response_buffer);

                CAN_sequence_state = CAN_state_idle;
                CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_COMPLETE;
                CAN_in_CTL = false;

                job_clear_timeout(SYS_CAN);

            } else {
                CAN_read_voltage_subsequence_state = CAN_read_voltage_subsequence_FAIL;
                CAN_control(response_buffer);

            }

            break;

        case CAN_read_voltage_subsequence_FAIL:
        default:
            CAN_sequence_state = CAN_state_idle;
            CAN_in_CTL = false;
            job_clear_timeout(SYS_CAN);
            menu_send_CTL();
            USB_tx_string_P(PSTR("CAN read voltage fail\r\n>"));
            break;
    }
}

void CAN_hear_chatter_start(void) {
    if (CAN_sequence_state == CAN_state_idle) {
        CAN_sequence_state = CAN_state_hear_chatter;
        CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_1;
		CAN_last_did_hear_chatter = false;
		CAN_chatter_count = 0;
        CAN_in_CTL = true; //make sure responses come back to command processor
        char hold[2] = "\0"; // ??
        CAN_control (hold);
    } else {
        CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_FAIL;
    }
}

/**
 * Parses the buffer for a valid voltage reading
 * @param buffer
 * @return the voltage reading, or -1 if there's an error
 */
void CAN_parse_chatter(char * buffer);
void CAN_parse_chatter(char * buffer) {
	if (CAN_last_did_hear_chatter == false) { // && TODO check if hex byte or something?
		CAN_chatter_count++;
	}
	
	if (CAN_chatter_count > CAN_HEARD_CHATTER_CUTOFF) {
		CAN_last_did_hear_chatter = true;
	}
}

void CAN_hear_chatter_sequence (char * response_buffer) {
	static Bool heardEcho = false;
	
    switch (CAN_hear_chatter_subsequence_state) {
        case CAN_hear_chatter_subsequence_1:
            CTL_add_string_to_buffer_P(&CAN, BUFFER_OUT, PSTR("ATSP0\r")); //compose message
            CTL_mark_for_processing(&CAN, BUFFER_OUT); //send it

            CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_2; //move to response state
            job_set_timeout(SYS_CAN, 2);

            break;

        case CAN_hear_chatter_subsequence_2: //Module response
            if (strcmp_P(response_buffer, PSTR("ATSP0")) == 0) {
				heardEcho = true;
				
            } else if (strcmp_P(response_buffer, PSTR("OK")) == 0 && heardEcho) {
				heardEcho = false;
				
				CTL_add_string_to_buffer_P(&CAN, BUFFER_OUT, PSTR("ATMA\r")); //compose message
				CTL_mark_for_processing(&CAN, BUFFER_OUT); //send it

				CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_3;
				job_set_timeout(SYS_CAN, 1);

            } else {
                CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_FAIL;
                CAN_control(response_buffer);

            }

            break;

        case CAN_hear_chatter_subsequence_3:
            if (strcmp_P(response_buffer, PSTR("ATMA")) == 0) {
				//heardEcho = true;
				
			} else if (strcmp_P(response_buffer, PSTR(".timeout.")) == 0) {
				CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_4;
				CAN_control(response_buffer);
			
			} if (strcmp_P(response_buffer, PSTR("SEARCHING...")) == 0) {
				; // Do nothing
			
			} else {
	            CAN_parse_chatter(response_buffer);

                if (CAN_last_did_hear_chatter) {
                    CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_4;
					char ng = "\n";
					CAN_control(&ng);
				}
			}			

			break;

		case CAN_hear_chatter_subsequence_4:
			CTL_add_string_to_buffer_P(&CAN, BUFFER_OUT, PSTR("\r")); //compose message
			CTL_mark_for_processing(&CAN, BUFFER_OUT); //send it

			CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_5;
			job_set_timeout(SYS_CAN, 1);

			break;

		case CAN_hear_chatter_subsequence_5:
            CAN_sequence_state = CAN_state_idle;
            CAN_hear_chatter_subsequence_state = CAN_hear_chatter_subsequence_COMPLETE;
            CAN_in_CTL = false;

            job_clear_timeout(SYS_CAN);

            break;

        case CAN_hear_chatter_subsequence_FAIL:
        default:
            CAN_sequence_state = CAN_state_idle;
            CAN_in_CTL = false;
            job_clear_timeout(SYS_CAN);

            menu_send_CTL();
            USB_tx_string_P(PSTR("CAN hear chatter fail\r\n>"));

            break;
    }
}

uint8_t CAN_get_sequence_state(void) {
    return CAN_sequence_state;
}

uint8_t CAN_get_init_subsequence_state(void) {
    return CAN_init_subsequence_state;
}

uint8_t CAN_get_ee_subsequence_state(void) {
    return CAN_ee_subsequence_state;
}

uint8_t CAN_get_read_voltage_subsequence_state(void) {
	return CAN_read_voltage_subsequence_state;
}

uint8_t CAN_get_hear_chatter_subsequence_state(void) {
	return CAN_hear_chatter_subsequence_state;
}

double CAN_get_last_read_voltage(void) {
    return (CAN_last_read_voltage + READ_VOLTAGE_DIODE_DROP);
}

Bool CAN_get_last_did_hear_chatter(void) {
    return  CAN_last_did_hear_chatter;
}

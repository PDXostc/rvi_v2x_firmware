/*
 * V2X_gsm.c
 *
 * Created: 2/12/2016 11:01:08 AM
 *  Author: jbanks2
 */

#include "V2X.h"


char imei[16] = "\0";
char latitude[13] = "\0";
char latitude_hemispere[2] = "\0";
char longitude[13] = "\0";
char longitude_hemispere[2] = "\0";

volatile buff GSM;
char stng[100] = "\0";
int GSM_sequence_state = GSM_state_idle;
int GSM_subsequence_state = GSM_subssequence_FAIL;

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

 	clear_buffer(stng);
	GSM.input_proc_flag = GSM.input_proc_index = GSM.input_index = 0;
	GSM.output_proc_active_flag = GSM.output_proc_index = 0;
	CTL_add_to_buffer(&GSM, BUFFER_IN, '\r'); //put something in buffer so pointers are different
}


void GSM_set_tx_int(void) {
	USART_SIM.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_HI_gc;
}

void GSM_clear_tx_int(void) {
	USART_SIM.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_OFF_gc;
}


void GSM_add_string_to_buffer(Bool in_out, char * to_add) {
	CTL_add_string_to_buffer(&GSM, in_out, to_add);
}

void GSM_mark_for_processing(Bool in_out) {
	CTL_mark_for_processing(&GSM, in_out);
}

ISR(USART_SIM_RX_Vect)
{
	char value = USART_SIM.DATA;
	CTL_add_to_buffer(&GSM, BUFFER_IN, value);
	if (value == '\n' || value == '\r' ) {
		GSM.input_proc_flag = true;
	}
}

ISR(USART_SIM_DRE_Vect)
{
	if (CTL_bytes_to_send(&GSM, BUFFER_OUT)) { //if bytes left to send
		USART_SIM.DATA = CTL_next_byte(&GSM, BUFFER_OUT);
	} else { //clean up
		GSM_clear_tx_int();
		GSM.output_proc_loaded = false; //mark string as sent
	}
}

void GSM_modem_init (void) {
	if (GSM_sequence_state == GSM_state_idle) {
		GSM_sequence_state = GSM_power_check;
		GSM_subsequence_state = GSM_subssequence_1; //move to response state
		char hold[2];
		GSM_control (hold);
	}
}

void GSM_time_job (void) {
	if (GSM_sequence_state == GSM_state_idle) {
		GSM_sequence_state = GSM_state_time_get;
		GSM_subsequence_state = GSM_subssequence_1; //move to response state
		char hold[2];
		GSM_control (hold);
	}
}

void GSM_control (char * responce_buffer) {
	switch (GSM_sequence_state) {
		case GSM_state_idle:
		break;
		case GSM_power_check:
		GSM_control_check(responce_buffer);
		break;
		case GSM_state_start:
		GSM_control_start(responce_buffer);
		break;
		case GSM_state_time_get:
		GSM_time_sync(responce_buffer);
		break;
		default:
		GSM_sequence_state = GSM_state_idle;
	}
}

void GSM_control_check (char * responce_buffer){
	switch (GSM_subsequence_state) {
	case GSM_subssequence_1:  //check module power
		if (PWR_query((1<<ENABLE_4V1))) { //is the module power on?
			GSM_subsequence_state = GSM_subssequence_3; 
			GSM_control_check(responce_buffer);
		} else { //if not power it up
			usb_tx_string_P(PSTR("\rCTL>>>:Power up GSM\r"));  //does not need end of string, exits through menu
			PWR_gsm_start();
			GSM_subsequence_state = GSM_subssequence_2;
			job_set_timeout(SYS_GSM, 10); //give SIM module 10 seconds to start
		}
		break;
	case GSM_subssequence_2: //Module clean boot, look for "start"
		if (strcmp_P(responce_buffer, PSTR("START")) == 0) {
			GSM_subsequence_state = GSM_subssequence_3;  //got expected response, go to next step
			GSM_control_check(responce_buffer);
		}
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_3: //check for SIM power LED state
		if (sim_power_status()) {
			menu_send_CTL();
			usb_tx_string_P(PSTR("GSM is powered\r>"));
			GSM_subsequence_state = GSM_subssequence_6;
			GSM_control_check(responce_buffer);
		} else { //try a reset
			menu_send_CTL();
			usb_tx_string_P(PSTR("GSM rebooting\r>"));
			PWR_gsm_stop();
			PWR_gsm_start();
			GSM_subsequence_state = GSM_subssequence_2;
			job_set_timeout(SYS_GSM, 10); //give SIM module 10 seconds to start
		}
		break;
	case GSM_subssequence_6: //check for command responce
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("AT\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT);
		GSM_subsequence_state = GSM_subssequence_7; //move to response state
		job_set_timeout(SYS_GSM, 2); //give SIM module 2 seconds to restart
		break;
	case GSM_subssequence_7:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0) {
			menu_send_CTL();
			usb_tx_string_P(PSTR("GSM Responding\r>"));
			GSM_subsequence_state = GSM_subssequence_1;  //got expected response, go to next step
			GSM_sequence_state = GSM_state_start;
			GSM_control(responce_buffer); //start next state
		}
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_FAIL:
	default:
		GSM_sequence_state = GSM_state_idle;
		job_clear_timeout(SYS_GSM);
		menu_send_CTL();
		usb_tx_string_P(PSTR("Could not connect to GSM\r>"));
		break;
	}
}

void GSM_control_start (char * responce_buffer){
	switch (GSM_subsequence_state) {
	case GSM_subssequence_1:
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("ATE0\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT);
		GSM_subsequence_state = GSM_subssequence_2; //move to response state
		job_set_timeout(SYS_GSM, 2);
		break;
	case GSM_subssequence_2:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0) {
			menu_send_CTL();
			usb_tx_string_P(PSTR("GSM Echo off\r>"));
			GSM_subsequence_state = GSM_subssequence_3; //move to response state
			GSM_control(responce_buffer);
		}
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_3:
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("ATI\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT); //send it
		GSM_subsequence_state = GSM_subssequence_4; //move to response state
		job_set_timeout(SYS_GSM, 2);
		break;
	case GSM_subssequence_4:  //get device information
		if (strcmp_P(responce_buffer, PSTR("Model: SIMCOM_SIM5320A")) == 0) {
			menu_send_CTL();
			usb_tx_string_P(PSTR("SIM5320A device detected\r>"));
			job_set_timeout(SYS_GSM, 2);
			GSM_subsequence_state = GSM_subssequence_5;  //got expected response, go to next step
		} else if (strcmp_P(responce_buffer, PSTR("Model: SIMCOM_SIM7100A")) == 0) { //got new version of simcom chip
			menu_send_CTL();
			usb_tx_string_P(PSTR("SIM7100A device detected\r>"));
			job_set_timeout(SYS_GSM, 2);
			GSM_subsequence_state = GSM_subssequence_5;  //got expected response, go to next step
		}	else if (strcmp_P(responce_buffer, PSTR("OK")) == 0) {	//did not see matching device ID
			GSM_subsequence_state = GSM_subssequence_FAIL;
			job_clear_timeout(SYS_GSM);
			GSM_control(responce_buffer);
		}  //else {keep looking}
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_5:
		clear_buffer(stng);
        /* FIXME: operational divergence SIMCOM 5320a vs 7100a 
		 * 5320a stores prints out "IMEI: ..."
		 * 7100a prints out "IMEISV: ..."
		 */
#if SIMCOM == SIMCOM_SIM5320A
		for (int i = 0; i < 5; i++) {stng[i] = responce_buffer[i];} //move first 4 to compare
		if (strcmp_P(stng, PSTR("IMEI:")) == 0) {
			clear_buffer(imei);
			strcat(imei, responce_buffer+6);
			menu_send_CTL();
			usb_tx_string_P(PSTR("IMEI captured "));
			menu_send_n_st();
			job_set_timeout(SYS_GSM, 2);
			GSM_subsequence_state = GSM_subssequence_6;  //got expected response, go to next step
		}
#elif SIMCOM == SIMCOM_SIM7100A
		for (int i = 0; i < 7; i++) {stng[i] = responce_buffer[i];} //move first 6 to compare
		if (strcmp_P(stng, PSTR("IMEISV:")) == 0) {
			clear_buffer(imei);
			strcat(imei, responce_buffer+8);
			menu_send_CTL();
			usb_tx_string_P(PSTR("IMEISV captured "));
			menu_send_n_st();
			job_set_timeout(SYS_GSM, 2);
			GSM_subsequence_state = GSM_subssequence_6;  //got expected response, go to next step
		}
#endif
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_6:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0){
			menu_send_CTL();
			usb_tx_string_P(PSTR("GSM Started\r>"));
			CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("AT+CGPS=1\r")); //compose message
			CTL_mark_for_processing(&GSM, BUFFER_OUT); //send it
			GSM_subsequence_state = GSM_subssequence_7;
			job_set_timeout(SYS_GSM, 2);
		}  //else {keep looking}
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_7:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0){
			menu_send_CTL();
			usb_tx_string_P(PSTR("GPS Started\r>"));
			GSM_sequence_state = GSM_state_idle;
			job_clear_timeout(SYS_GSM);
		}  //else {keep looking}
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_FAIL:
	default:
		menu_send_CTL();
		usb_tx_string_P(PSTR("GSM Start failure\r>"));
		GSM_sequence_state = GSM_state_idle;
		job_clear_timeout(SYS_GSM);
		break;
	}
}

void GSM_process_buffer (void) {
	if (GSM.output_proc_active_flag) {
		if (GSM.output_proc_loaded) { //output buffer is ready to send
			GSM_set_tx_int();		//set ISR flag
		} else {
			CTL_purge_buffer(&GSM, BUFFER_OUT);
		}
	}
	while (GSM.input_proc_flag) {
		CTL_copy_to_proc(&GSM); //copy string from buffer
		if (GSM.input_proc_loaded) { //process string
			menu_send_GSM();
			usb_cdc_send_string(USB_CMD, GSM.input_proc_buf);
			menu_send_n_st();
			GSM_control(GSM.input_proc_buf);
			GSM.input_proc_loaded = false;		//input proc buffer has been handled
		}
	}
}

void GSM_time_sync (char * responce_buffer) {
	switch (GSM_subsequence_state) {
	case GSM_subssequence_1:
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("AT+CGPSINFO\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT);
		GSM_subsequence_state = GSM_subssequence_2; //move to response state
		job_set_timeout(SYS_GSM, 2);
		break;
	case GSM_subssequence_2:
		for (int i = 0; i < 10; i++) {stng[i] = responce_buffer[i];} //move first 10 to compare
		if (strcmp_P(stng, PSTR("+CGPSINFO:")) == 0 && responce_buffer[10] != ',') {
			GSM_parse_gps_info(responce_buffer);
			GSM_subsequence_state = GSM_subssequence_3;
			job_set_timeout(SYS_GSM, 2);
		}
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_3:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0) {
			GSM_sequence_state = GSM_state_idle;

			job_clear_timeout(SYS_GSM);
		}
		job_check_fail(SYS_GSM);
		break;
	case GSM_subssequence_FAIL:
	default:
		menu_send_CTL();
		usb_tx_string_P(PSTR("GPS time update FAIL\r>"));
		GSM_sequence_state = GSM_state_idle;
		job_clear_timeout(SYS_GSM);
		break;
	}
}

void show_buffer(char * buffer) {
	usb_tx_string_P(PSTR("\""));
	usb_cdc_send_string(USB_CMD, buffer);
	usb_tx_string_P(PSTR("\""));

}

void GSM_parse_gps_info (char * responce_buffer) {

	char * start_ptr = strchr(responce_buffer, ':') + 1;
	for (int i = 0; start_ptr[i] != ','; i++) {
		latitude[i] = start_ptr[i];	//copy lat string
	}
	start_ptr = strchr(start_ptr, ',') + 1;
	for (int i = 0; start_ptr[i] != ','; i++) {
		latitude_hemispere[i] = start_ptr[i];	//copy lat hemi string
	}
	start_ptr = strchr(start_ptr, ',') + 1;
	for (int i = 0; start_ptr[i] != ','; i++) {
		longitude[i] = start_ptr[i];	//copy long string
	}
	start_ptr = strchr(start_ptr, ',') + 1;
	for (int i = 0; start_ptr[i] != ','; i++) {
		longitude_hemispere[i] = start_ptr[i];	//copy long hemi string
	}
	start_ptr = strchr(start_ptr, ',') + 1;
	char date[10] = "\0";
	for (int i = 0; start_ptr[i] != ','; i++) {
		date[i] = start_ptr[i];	//copy date string
	}
	start_ptr = strchr(start_ptr, ',') + 1;
	char time[10] = "\0";
	for (int i = 0; start_ptr[i] != ','; i++) {
		time[i] = start_ptr[i];	//copy time string
	}

	if (atoi(date) != 0) { //this logic wont work on Jan 1 2100
		time_set_by_strings(date, time);
		menu_send_CTL();
		usb_tx_string_P(PSTR("GPS time sync @ "));
		time_print_human_readable();
		menu_send_n_st();
	}
}

void GSM_control_fail (void) {
	GSM_subsequence_state = GSM_subssequence_FAIL;
}

char * GSM_get_imei (void) {
	return &imei;
}
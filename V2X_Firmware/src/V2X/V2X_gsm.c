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
char hold[100] = "\0";

volatile buff GSM;
char stng[100] = "\0";
int GSM_sequence_state = GSM_state_idle;
int GSM_subsequence_state = GSM_subssequence_FAIL;

uint32_t job_timeout = 0;
Bool job_timeout_enable = false;

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

	GSM.in_store_a = GSM.in_store_b = GSM.out_store_a = GSM.out_store_b = 0;
	GSM.in_proc_a = GSM.in_proc_b = GSM.out_proc_a = GSM.out_proc_b = 0;
	GSM.active_in = GSM.active_out = BUFFER_A;	

	CTL_add_to_buffer(&GSM, BUFFER_IN, '\n'); //A
	CTL_mark_for_processing(&GSM, BUFFER_IN);
	CTL_purge_buffer(&GSM, BUFFER_IN);
	CTL_add_to_buffer(&GSM, BUFFER_IN, '\n');  //B
	CTL_mark_for_processing(&GSM, BUFFER_IN);
	CTL_purge_buffer(&GSM, BUFFER_IN);

	CTL_add_to_buffer(&GSM, BUFFER_OUT, '\n');
	CTL_mark_for_processing(&GSM, BUFFER_OUT);
	CTL_purge_buffer(&GSM, BUFFER_OUT);
	CTL_add_to_buffer(&GSM, BUFFER_OUT, '\n');
	CTL_mark_for_processing(&GSM, BUFFER_OUT);
	CTL_purge_buffer(&GSM, BUFFER_OUT);
	
	clear_buffer(stng);
}

void GSM_set_tx_int(void) {
	USART_SIM.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_HI_gc;
}

void GSM_clear_tx_int(void) {
	USART_SIM.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t) USART_DREINTLVL_OFF_gc;
}

void GSM_process_buffer (void) {
	// if there is something received
	if (CTL_bytes_to_send(&GSM, BUFFER_IN)) {
		int i = 0;
		while (CTL_bytes_to_send(&GSM, BUFFER_IN)) {
			hold[i++] = CTL_next_byte(&GSM, BUFFER_IN);
		}
		hold[i] = '\0';
		CTL_purge_buffer(&GSM, BUFFER_IN);
		usb_tx_string_P(PSTR("GSM>>>:"));
		usb_cdc_send_string(USB_CMD, hold);
		menu_send_n_st();
		GSM_control(hold); //process messages if in sleep mode
	}
	if (CTL_bytes_to_send(&GSM, BUFFER_OUT)) {
		GSM_set_tx_int(); //set to continue sending buffer
	}
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
	if (value == '\n' || value == '\r' ) {
		CTL_mark_for_processing(&GSM, BUFFER_IN);
	} else {
		CTL_add_to_buffer(&GSM, BUFFER_IN, value);
	}
}

ISR(USART_SIM_DRE_Vect)
{
	if (CTL_bytes_to_send(&GSM, BUFFER_OUT)) { //if bytes left to send
		USART_SIM.DATA = CTL_next_byte(&GSM, BUFFER_OUT);
	} else { //clean up
		CTL_purge_buffer(&GSM, BUFFER_OUT);
		GSM_clear_tx_int();
	}
}

void GSM_begin_init (void) {
	if (GSM_sequence_state == GSM_state_idle) {
	GSM_sequence_state = GSM_state_check;
	GSM_subsequence_state = GSM_subssequence_1; //move to response state
	GSM_control (hold);
	}
}

void GSM_time_job (void) {
	if (GSM_sequence_state == GSM_state_idle) {
		GSM_sequence_state = GSM_state_time_get;
		GSM_subsequence_state = GSM_subssequence_1; //move to response state
		GSM_control (hold);
	}
}

void GSM_control (char * responce_buffer) {
	switch (GSM_sequence_state) {
	case GSM_state_idle:
		break;
	case GSM_state_check:
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
//	char stng[10] = "\0";
	switch (GSM_subsequence_state) {
	case GSM_subssequence_1:  //check module power
		if (power_query((1<<ENABLE_4V1))) { //is the module power on?
			GSM_subsequence_state = GSM_subssequence_3; 
			GSM_control_check(responce_buffer);
		} else { //if not power it up
			usb_tx_string_P(PSTR("CTL>>>:Power up GSM\r>"));
			power_control_turn_on((1<<ENABLE_4V1));
			power_control_push();
			delay_ms(10);
			power_sim_start();
			GSM_subsequence_state = GSM_subssequence_2;
			job_set_timeout(10); //give SIM module 10 seconds to start
		}
		break;
	case GSM_subssequence_2: //Module clean boot, look for "start"
		if (strcmp_P(responce_buffer, PSTR("START")) == 0) {
			GSM_subsequence_state = GSM_subssequence_3;  //got expected response, go to next step
			GSM_control_check(responce_buffer);
		} 
		job_check_fail();
		break;
	case GSM_subssequence_3: //check for SIM power LED state
		if (sim_power_status()) {
			usb_tx_string_P(PSTR("CTL>>>:GSM is powered\r>"));
			GSM_subsequence_state = GSM_subssequence_6;
			GSM_control_check(responce_buffer);
		} else { //try a reset
			usb_tx_string_P(PSTR("CTL>>>:Trying to reboot GSM\r>"));
			power_sim_stop(); 
			power_sim_start();
			GSM_subsequence_state = GSM_subssequence_2;
			job_set_timeout(10); //give SIM module 10 seconds to start
		}
		break;
	case GSM_subssequence_6: //check for command responce
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("AT\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT);
		GSM_subsequence_state = GSM_subssequence_7; //move to response state
		job_set_timeout(2); //give SIM module 2 seconds to restart
		break;
	case GSM_subssequence_7:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0) {
			usb_tx_string_P(PSTR("CTL>>>:GSM responding to commands\r>"));
			GSM_subsequence_state = GSM_subssequence_1;  //got expected response, go to next step
			GSM_sequence_state = GSM_state_start;
			GSM_control(responce_buffer); //start next state
		} 
// 		else {
// 			usb_tx_string_P(PSTR("\""));
// 			usb_cdc_send_string(USB_CMD, responce_buffer);
// 			usb_tx_string_P(PSTR("\"\r>"));
// 		}
		job_check_fail();
		break;
	case GSM_subssequence_FAIL:
	default:
		GSM_sequence_state = GSM_state_idle;
		job_clear_timeout();
		usb_tx_string_P(PSTR("CTL>>>:Could not connect to GSM\r>"));
		break;
	}
}

void GSM_control_start (char * responce_buffer){
	switch (GSM_subsequence_state) {
	case GSM_subssequence_1:
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("ATE0\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT);
		GSM_subsequence_state = GSM_subssequence_2; //move to response state
		job_set_timeout(2);
		break;
	case GSM_subssequence_2:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0) {
			usb_tx_string_P(PSTR("CTL>>>:Echo off\r>"));
			GSM_subsequence_state = GSM_subssequence_3; //move to response state
			GSM_control(responce_buffer);
		} 
		job_check_fail();		
		break;
	case GSM_subssequence_3:
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("ATI\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT); //send it
		GSM_subsequence_state = GSM_subssequence_4; //move to response state
		job_set_timeout(2);
		break;
	case GSM_subssequence_4:  //get device information
		if (strcmp_P(responce_buffer, PSTR("Model: SIMCOM_SIM5320A")) == 0) {
			usb_tx_string_P(PSTR("CTL>>>:SIM5320A device detected\r>"));
			GSM_subsequence_state = GSM_subssequence_5;  //got expected response, go to next step
		} else if (strcmp_P(responce_buffer, PSTR("OK")) == 0){
			//did not see matching device ID
			GSM_subsequence_state = GSM_subssequence_FAIL;
			job_clear_timeout();
			GSM_control(responce_buffer);
		}  //else {keep looking}
		job_check_fail();
		break;
	case GSM_subssequence_5:
		for (int i = 0; i < 4; i++) {stng[i] = responce_buffer[i];} //move first 4 to compare
		if (strcmp_P(stng, PSTR("IMEI")) == 0) {
			clear_buffer(imei);
			strcat(imei, responce_buffer+6);
			GSM_subsequence_state = GSM_subssequence_6;  //got expected response, go to next step
		}
		job_check_fail();
		break;
	case GSM_subssequence_6:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0){
			usb_tx_string_P(PSTR("CTL<<<IMEI:"));
			usb_cdc_send_string(USB_CMD, imei);
			menu_send_n_st();
			GSM_subsequence_state = GSM_subssequence_7;
			GSM_control(responce_buffer);
		}  //else {keep looking}
		job_check_fail();
		break;
	case GSM_subssequence_7:
		usb_tx_string_P(PSTR("CTL>>>:SIM Start end\r>"));
		GSM_sequence_state = GSM_state_idle;
		job_clear_timeout();
		break;
	case GSM_subssequence_FAIL:
	default:
		usb_tx_string_P(PSTR("CTL>>>SIM Start FAULT\r>"));
		GSM_sequence_state = GSM_state_idle;
		job_clear_timeout();
		break;
	}
}

void GSM_time_sync (char * responce_buffer) {
	switch (GSM_subsequence_state) {
	case GSM_subssequence_1:
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("AT+CGPSINFO\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT);
		GSM_subsequence_state = GSM_subssequence_2; //move to response state
		job_set_timeout(2);
		break;
	case GSM_subssequence_2:
		for (int i = 0; i < 10; i++) {stng[i] = responce_buffer[i];} //move first 10 to compare
		//show_buffer(stng);
		if (strcmp_P(stng, PSTR("+CGPSINFO:")) == 0) {
			GSM_subsequence_state = GSM_subssequence_3;
			GSM_parse_gps_info(responce_buffer);
			//expecting other lines before "ok"
			job_set_timeout(2);
		}
		job_check_fail();
		break;
	case GSM_subssequence_3:
		if (strcmp_P(stng, PSTR("OK")) == 0) {
			GSM_sequence_state = GSM_state_idle;
		} else {
			usb_cdc_send_string(USB_CMD, responce_buffer);
		}
		job_check_fail();
		break;
	case GSM_subssequence_FAIL:
	default:
		usb_tx_string_P(PSTR("CTL>>>GPS time update FAIL\r>"));
		GSM_sequence_state = GSM_state_idle;
		job_clear_timeout();
		break;
	}
}

void show_buffer(char * buffer) {
	usb_tx_string_P(PSTR("\""));
	usb_cdc_send_string(USB_CMD, buffer);
	usb_tx_string_P(PSTR("\""));
	
}

// 	>GSM>>>:+CGPSINFO:4532.003283,N,12241.160752,W,180316,211341.1,-9.2,
// 	>GSM>>>:AmpI/AmpQ: 483/460
// 	>GSM>>>:OK
// 	>GSM>>>:+STIN: 25
// 	>
void GSM_parse_gps_info (char * responce_buffer) {
	
	char * start_ptr = strchr(responce_buffer, ':') + 1;
	for (int i = 0; start_ptr[i] != ','; i++) {
		latitude[i] = start_ptr[i];	//copy lat string
	}
	start_ptr = strchr(start_ptr+1, ',') + 1;
	for (int i = 0; start_ptr[i] != ','; i++) {
		latitude_hemispere[i] = start_ptr[i];	//copy lat hemi string
	}
	start_ptr = strchr(start_ptr+1, ',') + 1;
	for (int i = 0; start_ptr[i] != ','; i++) {
		longitude[i] = start_ptr[i];	//copy long string
	}
	start_ptr = strchr(start_ptr+1, ',') + 1;
	for (int i = 0; start_ptr[i] != ','; i++) {
		longitude_hemispere[i] = start_ptr[i];	//copy long hemi string
	}
	start_ptr = strchr(start_ptr+1, ',') + 1;
	char date[10] = "\0";
	for (int i = 0; start_ptr[i] != ','; i++) {
		date[i] = start_ptr[i];	//copy date string
	}
	start_ptr = strchr(start_ptr+1, ',') + 1;
	char time[10] = "\0";
	for (int i = 0; start_ptr[i] != ','; i++) {
		time[i] = start_ptr[i];	//copy time string
	}
	
	usb_tx_string_P(PSTR("CTL>>>:Time sync start: "));
	print_human_time();
		
/*	usb_tx_string_P(PSTR("lat = "));
	show_buffer(latitude);
	menu_send_n_st();
	usb_tx_string_P(PSTR("lat_h = "));
	show_buffer(latitude_hemispere);
	menu_send_n_st();
	usb_tx_string_P(PSTR("long = "));
	show_buffer(longitude);
	menu_send_n_st();
	usb_tx_string_P(PSTR("long_h = "));
	show_buffer(longitude_hemispere);
	menu_send_n_st();
	usb_tx_string_P(PSTR("time = "));
	show_buffer(time);
	menu_send_n_st();
	usb_tx_string_P(PSTR("date = "));
	show_buffer(date);
	menu_send_n_st();
	*/
	if (atoi(date) != 0) { //this logic wont work on Jan 1 2100
		time_set_by_strings(date, time);
		usb_tx_string_P(PSTR("CTL>>>:Time updated to: "));
		print_human_time();
		menu_send_n_st();
	} else {	
		usb_tx_string_P(PSTR("CTL>>>:Time not updated\r>"));
	}
}

void job_set_timeout (int span) {
	job_timeout = time_get() + span;
	job_timeout_enable = true;
}

Bool job_check_timeout(void) { //returns true if timeout has elapsed
	if (time_get() > job_timeout && job_timeout_enable) {
		return true;
		} else {return false;}
}

void job_clear_timeout (void) {
	job_timeout_enable = false;
}

void job_check_fail(void) {
	if (job_check_timeout()) {//has the controller been called by a timeout?
		job_clear_timeout();
		GSM_subsequence_state = GSM_subssequence_FAIL;
		job_set_timeout(1);
		usb_tx_string_P(PSTR("CTL>>>:Response timeout\r>"));
	}
}

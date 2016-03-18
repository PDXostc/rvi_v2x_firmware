/*
 * V2X_gsm.c
 *
 * Created: 2/12/2016 11:01:08 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"


char imei[16] = "\0";
volatile buff GSM;
char stng[10] = "\0";
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
		usb_tx_string_P(PSTR("GSM>>>:"));
		usb_cdc_send_string(USB_CMD, CTL_ptr_to_proc_buffer(&GSM, BUFFER_IN));
		usb_tx_string_P(PSTR("\r>"));
		GSM_control(CTL_ptr_to_proc_buffer(&GSM, BUFFER_IN)); //process messages if in sleep mode
		CTL_purge_buffer(&GSM, BUFFER_IN);
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
{	// Transfer UART RX fifo to buffer
// 	if (0 != (USART.STATUS & (USART_FERR_bm | USART_BUFOVF_bm))) {
// 		udi_cdc_multi_putc(USB_CAN, '!');
// 	}
	char value = USART_SIM.DATA;
	if (value == '\n' || value == '\r' ) {
		CTL_mark_for_processing(&GSM, BUFFER_IN);
		} else {
		CTL_add_to_buffer(&GSM, BUFFER_IN, value);
	}
}

ISR(USART_SIM_DRE_Vect)
{
	if (CTL_bytes_to_send(&GSM, BUFFER_OUT)) {
// 		uint8_t value = CTL_next_byte(&GSM, BUFFER_OUT);
// 		USART_SIM.DATA = value;
// 		usb_cdc_send_byte(USB_CMD, value);
		USART_SIM.DATA = CTL_next_byte(&GSM, BUFFER_OUT);
		} else {
		CTL_purge_buffer(&GSM, BUFFER_OUT);
		GSM_clear_tx_int();
	}
}

void GSM_begin_init (void) {
	if (GSM_sequence_state == GSM_state_idle) {
	GSM_sequence_state = GSM_state_check;
	GSM_subsequence_state = GSM_subssequence_1; //move to response state
	GSM_control (CTL_ptr_to_proc_buffer(&GSM, BUFFER_OUT));
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
			power_sim_start();
			GSM_subsequence_state = GSM_subssequence_2;
		}
		break;
	case GSM_subssequence_2: //Module clean boot, look for "start"
		if (strcmp_P(responce_buffer, PSTR("START")) == 0) {
			GSM_subsequence_state = GSM_subssequence_3;  //got expected response, go to next step
			GSM_control_check(responce_buffer);
		//expired timestamp check
		} 
		break;
	case GSM_subssequence_3: //check for SIM power LED state
		if (sim_power_status()) {
			GSM_subsequence_state = GSM_subssequence_6;
			usb_tx_string_P(PSTR("CTL>>>:GSM is powered\r>"));
			GSM_control_check(responce_buffer);
		} else { //try a reset
			usb_tx_string_P(PSTR("CTL>>>:Trying to reboot GSM\r>"));
			power_sim_stop(); 
			power_sim_start();
			GSM_subsequence_state = GSM_subssequence_2;
		}
		break;
	case GSM_subssequence_6: //check for command responce
		CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("AT\r")); //compose message
		CTL_mark_for_processing(&GSM, BUFFER_OUT);
		GSM_subsequence_state = GSM_subssequence_7; //move to response state
		break;
	case GSM_subssequence_7:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0) {
			GSM_subsequence_state = GSM_subssequence_1;  //got expected response, go to next step
			GSM_sequence_state = GSM_state_start;
			usb_tx_string_P(PSTR("CTL>>>:GSM responding to commands\r>"));
			GSM_control(responce_buffer); //start next state
		} else if (strcmp_P(responce_buffer, PSTR("AT")) == 0) {
			GSM_subsequence_state = GSM_subsequence_state;
		} else {
			GSM_subsequence_state = GSM_subssequence_FAIL;
		}
		break;
	case GSM_subssequence_FAIL:
	default:
		GSM_sequence_state = GSM_state_idle;
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
		break;
	case GSM_subssequence_2:
		if (strcmp_P(responce_buffer, PSTR("ATE0")) == 0) {
			GSM_subsequence_state = GSM_subsequence_state;  //got expected response, go to next step
		} else if (strcmp_P(responce_buffer, PSTR("OK")) == 0) {
			CTL_add_string_to_buffer_P(&GSM, BUFFER_OUT, PSTR("ATI\r")); //compose message
			CTL_mark_for_processing(&GSM, BUFFER_OUT); //send it
//			GSM_subsequence_state = GSM_subssequence_4; //move to response state
			//no need to detect model number yet
			GSM_subsequence_state = GSM_subssequence_5;
		} else {
			GSM_subsequence_state = GSM_subssequence_FAIL;
		}
		break;
	case GSM_subssequence_4:  //get device information
		if (strcmp_P(responce_buffer, PSTR("Model: SIMCOM_SIM5320A")) == 0) {
//			usb_tx_string_P(PSTR("CTL<<<:SIM5320A device detected\r>"));
			GSM_subsequence_state = GSM_subssequence_5;  //got expected response, go to next step
		} else if (strcmp_P(responce_buffer, PSTR("OK")) == 0){
			GSM_subsequence_state = GSM_subssequence_FAIL;
		}  //else {keep looking}
		break;
	case GSM_subssequence_5:
		for (int i = 0; i < 4; i++) {stng[i] = responce_buffer[i];} //move first 4 to compare
		if (strcmp_P(stng, PSTR("IMEI")) == 0) {
//			usb_tx_string_P(PSTR("CTL>>>:IMEI found\r>"));
//			for (int i = 0; i < 15; i++) {imei[i] = responce_buffer[i+6];}
			clear_buffer(imei);
			strcat(imei, responce_buffer+6);
			GSM_subsequence_state = GSM_subssequence_6;  //got expected response, go to next step
		}
		break;
	case GSM_subssequence_6:
		if (strcmp_P(responce_buffer, PSTR("OK")) == 0){
			usb_tx_string_P(PSTR("CTL<<<IMEI:"));
			usb_cdc_send_string(USB_CMD, imei);
			menu_send_n_st();
			GSM_subsequence_state = GSM_subssequence_FAIL;
			GSM_control_start(responce_buffer);
		}  //else {keep looking}
		break;
	case GSM_subssequence_7:
		break;
	
	case GSM_subssequence_FAIL:
	default:
		usb_tx_string_P(PSTR("END\r>"));
		GSM_subsequence_state = GSM_subssequence_1;
		GSM_sequence_state = GSM_state_idle;
		break;
	}
}

void show_buffer(char * buffer) {
	usb_tx_string_P(PSTR("\""));
	usb_cdc_send_string(USB_CMD, buffer);
	usb_tx_string_P(PSTR("\""));
	
}
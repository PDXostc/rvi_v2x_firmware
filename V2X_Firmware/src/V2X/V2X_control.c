/*
 * V2X_command.c
 *
 * Created: 3/15/2016 9:46:58 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

uint32_t job_timeout[SYS_NUM];
Bool job_timeout_enable[SYS_NUM];

void CTL_add_to_buffer(buff * buffer, Bool in_out, char value) {
	switch (in_out) {
	case BUFFER_IN:
		buffer->input_buf[buffer->input_index++] = value;
		CTL_input_index_check(buffer);
		break;
	case BUFFER_OUT:
		buffer->output_proc_buf[buffer->output_proc_index++] = value;
		break;
	}
}

void CTL_add_string_to_buffer_P(buff * buffer, Bool in_out, const char * to_add) {
	int len = strlen_P(to_add);
	while (len != 0) {
		CTL_add_to_buffer(buffer, in_out, pgm_read_byte(to_add++));
		len--;
	}
}

void CTL_add_string_to_buffer(buff * buffer, Bool in_out, char * to_add) {
	int len = strlen(to_add);
	while (len != 0) {
		CTL_add_to_buffer(buffer, in_out, to_add[0]);
		to_add++;
		len--;
	}
}

int CTL_bytes_to_send (buff * buffer, Bool in_out) {
	int value;
	switch (in_out) {
	case BUFFER_IN:
		return strlen((buffer->input_proc_buf+buffer->input_proc_index));
		break;
	case BUFFER_OUT:
		return strlen((buffer->output_proc_buf+buffer->output_proc_index)); 
		break;
	}
}

char CTL_next_byte (buff * buffer, Bool in_out) {
	char value;
	switch (in_out) {
	case BUFFER_IN:
		value = buffer->input_proc_buf[buffer->input_proc_index++];
		CTL_input_proc_index_check(buffer);
		return value;
		break;
	case BUFFER_OUT:
		return buffer->output_proc_buf[buffer->output_proc_index++];
		break;
	}
}

void CTL_purge_buffer(buff * buffer, Bool in_out) {
	switch (in_out) {
	case BUFFER_IN:
		clear_buffer(buffer->input_proc_buf);
		break;
	case BUFFER_OUT:
		clear_buffer(buffer->output_proc_buf);
		buffer->output_proc_index = 0;
		buffer->output_proc_active_flag = false;
		buffer->output_proc_loaded = false;
		break;
		}
}

void clear_buffer(char * buffer) {
	int cnt = strlen(buffer);
	for(int i = 0; i < cnt; i++)
		{buffer[i] = '\0';}
}

void CTL_mark_for_processing(buff * buffer, Bool in_out) {
	switch (in_out) {
	case BUFFER_IN:
		buffer->input_proc_flag = true;
		break;
	case BUFFER_OUT:
		buffer->output_proc_active_flag = true;
		buffer->output_proc_loaded = true;
		buffer->output_proc_index = 0; //reset index for call out
		break;
	}
}

char * CTL_ptr_to_proc_buffer(buff * buffer, Bool in_out) {
	switch (in_out) {
	case BUFFER_IN:
		return (*buffer).input_proc_buf;
		break;
	case BUFFER_OUT:
		return (*buffer).output_proc_buf;
		break;
	}
}

void CTL_copy_to_proc(buff * buffer) { //enters with buffer->input_proc_flag true
 	CTL_purge_buffer(buffer, BUFFER_IN);  //only input is double buffered
	int i = 0; //var for moving through proc buffer
	uint16_t t_buf = buffer->input_proc_index; //save proc index so it can be reset if incomplete string
	while  (buffer->input_proc_flag && buffer->input_proc_index != buffer->input_index) { //while still processing
		char temp = buffer->input_buf[buffer->input_proc_index++]; //get char from input array
 		CTL_input_proc_index_check(buffer);
		if ( temp == '\n' || temp == '\r' || temp == 0x07 || temp == '>') {//an end of string char has been found
			if (i != 0)	{
				buffer->input_proc_loaded = true; //set flag of buffer copied
				buffer->input_proc_flag = false; //clear flag to exit loop
			} 
		} else {  //just another char, move to proc buffer
			buffer->input_proc_buf[i++] = temp; 
		}
	}
	if (buffer->input_proc_loaded) { //a string was found
		buffer->input_proc_flag = true; //set flag to cause buffer to be evaluated again
	}
	if (buffer->input_proc_index == buffer->input_index) {  //at end of circular buffer without end char, partial string
		buffer->input_proc_loaded = false; //set flag no buffer copied
		buffer->input_proc_flag = false; //clear flag to exit loop
		buffer->input_proc_index = t_buf; //reset pointer to beginning of unfinished string
	}
}

void CTL_input_index_check (buff * buffer) {
	if (buffer->input_index == sizeof(buffer->input_buf)) {
		buffer->input_index = 0; //reset pointer
	}
}

void CTL_input_proc_index_check (buff * buffer) {
	if (buffer->input_proc_index == sizeof(buffer->input_buf)) {
		buffer->input_proc_index = 0; //reset pointer
	}
}

void job_coordinator (void) {
	uint32_t temp_time = time_get();
	if (temp_time % 60 == 0) { //check every minute
		if (time_is_current() == 0) { //needs a sync
			GSM_time_job(); //kick off a time sync job
		}
	}

	if (job_check_timeout(SYS_GSM)) {
		char nuthin[] = ".timeout.";
		GSM_control(nuthin);  //start another job to catch the timeout
	}

	if (job_check_timeout(SYS_CAN)) {
		char nuthin[] = ".timeout.";
		CAN_control(nuthin);  //start another job to catch the timeout
	}

	if (job_check_timeout(SYS_CAN_CTL)) {
		CAN_stop_snoop();
	}

	if (job_check_timeout(SYS_PWR))
	{
		handle_button_check(button_get_delta());
		job_clear_timeout(SYS_PWR);
	}

	if (job_check_timeout(SYS_CAR_STATE_CHECK))
	{
        CSC_car_state_check();
	}

	//more jobs to add
	//compare GPS coordinates to trigger alarm/host
	//compare ACL data to trigger alarm/host
	//ignition detection to trigger alarm/host
}

void job_timeout_init () {
	for (uint8_t x = 0; x < SYS_NUM; x++) {
		//job_timeout[x] = time_get() //time does not matter if not being watched (Still a good practice to initialize variables to something as it helps prevent/find bugs down the road...)
		job_timeout[x] = 0;
		job_timeout_enable[x] = false;
	}

    /* Might need to turn on the car-state check */
    job_timeout_enable[SYS_CAR_STATE_CHECK] =
            (nvm_eeprom_read_byte(EE_car_state_check_enabled) == CSC_CAR_STATE_CHECK_ENABLED) ? true : false;
    
    if (job_timeout_enable[SYS_CAR_STATE_CHECK])
        job_set_timeout(SYS_CAR_STATE_CHECK, nvm_eeprom_read_byte(EE_car_state_check_high_power_check_interval));
}

void job_set_timeout (uint8_t system, int span) {
	job_timeout[system] = time_get() + span;
	job_timeout_enable[system] = true;
}

Bool job_check_timeout(uint8_t system) { //returns true if timeout has expired
	if (time_get() > job_timeout[system] && job_timeout_enable[system]) {
		return true;
	} else {return false;}
}

void job_clear_timeout (uint8_t system) {
	job_timeout_enable[system] = false;
}

void job_check_fail(uint8_t system) {
	if (job_check_timeout(system)) {//has the controller been called by a timeout?
		job_clear_timeout(system);
		switch (system) {
			case SYS_GSM:
			GSM_control_fail();
			menu_send_GSM();
			break;
			case SYS_CAN:
			CAN_control_init_fail();
			menu_send_CAN();
			break;
		}
		usb_tx_string_P(PSTR("Control timeout\r\n>"));
		job_set_timeout(system, 1);
	}
}


/*	switch (in_out) {
		case BUFFER_IN:
			
			break;
		case BUFFER_OUT:
			
			break;
	} */

/*	switch (in_out) {
		case BUFFER_IN:
			switch (buffer->active_in) {
				case BUFFER_A:
					break;
				case BUFFER_B:
					break;
			}
			break;
		case BUFFER_OUT:
			switch (buffer->active_out) {
				case BUFFER_A:
					break;
				case BUFFER_B:
					break;
			}
			break;
	} */


/*
 * V2X_menu.c
 *
 * Created: 2/23/2016 9:00:14 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"
#include "ctype.h"

char CMD_buffer [30] = "\0";
uint8_t sample[6] = {0,0,0,0,0,0};

void menu_add_to_command(char value) {
	if (value == 8 | value == 0x7f) {				//if backspace or delete
		CMD_buffer[strlen(CMD_buffer)-1] = '\0';	//null out the deleted char
	} else if (value > 0x20 && value < 0x7f || value == '+') {		//if in ascii set
		strcat(CMD_buffer, &value);					//store to buffer
	}
}

void menu_main(void) {
	//int cnt = strlen(CMD_buffer);
	for(int i = 0; i < 4; i++){
		CMD_buffer[i] = tolower(CMD_buffer[i]);
	}
	if (CMD_buffer[0] == 'v' && CMD_buffer[1] == 'x') {
		switch(CMD_buffer[2]) {
			case 'i': //information
				usb_tx_string_P(PSTR("Vehicle to Everything (V2X) RVI Node 2016\rOpen source hardware and software\rHW Rev1.2 \rSW Rev0.1\r"));
				break;
			case 'j': //Jaguar
				usb_tx_string_P(PSTR("   ,ggp@@@@mgg,,\r,$Q$(`S@@$;g$$$$$@$@@gg,\r;gP'$@gg)$$@@$@@@$(L$$||$$@g,\r  `g$P`  ``*%@@@P`)Eg|||lLLL||$Bgg,\r    `       ,gg$$@gg,`$..``$Z$$$$$EB$$@g,\r         @P`pgg$$$||`)gggg;,,     |$$$|$$$@g,\r         9w&    '*^^` ``*P#9BB00000$$$@|`$$$g|Sg,\r                                    *$@@L ```T$W~)%g,\r                                      *%@gg,,,,,    5/Sw,     ,\r                                          ```` ` `9g `9g,``*^|'\r                                                    `#g,`)h\r\r   Developed at Jaguar Land Rover OSCT. Portland OR 2016\r"));
				break;
			case 'q':
				menu_status();
				break;
			case 'r':  //reset
				usb_tx_string_P(PSTR("Reset function not available yet, please power cycle.\r"));
				break;
			case 'a':  //accel
				menu_accel();
				break;
			case 'c':  //CAN interface
				menu_can();
				break;
			case 's':  //CAN interface
				simcard_status();
				break;
			case 'm':  //modem
				menu_modem();
				break;
			case 'p':  //power
				menu_power();
				break;
			case 't':  //timer functions
				menu_timer();
				break;
			case '?':  //timer functions
			default:
				usb_tx_string_P(PSTR("*** Main Menu ***\rI: Device information\rA: Accelerometer\rC: CANbus\rM: Modem\rP: Power\rT: Timer\rQ: Query system status\rS: Simcard status\rR: Reboot\r"));
				break;
			}
	}else{
			usb_tx_string_P(PSTR("All commands start VX\r"));
	}
	clear_buffer(CMD_buffer);	//clear the buffer for next command	
	usb_tx_string_P(PSTR("\r>"));  //prompt for user input
}

void menu_accel (void) {
	int speed, data16;
	switch (CMD_buffer[3]) {
	case 'd':  //disable
		ACL_set_sample_off();
		usb_tx_string_P(PSTR("Accelerometer is off\r"));
		break;
	case 'e':  //enable
		ACL_set_sample_on();
		usb_tx_string_P(PSTR("Accelerometer has been started\r"));
		break;
	case 'i':
		usb_tx_string_P(PSTR("V2X uses the ADXL345 accelerometer from Analog Devices\r"));
		break;
	case 'q':
		menu_accel_status();
		break;
	case 'r':  //restart
		ACL_set_sample_off();
		ACL_take_sample(sample); //pushes "off" command
		ACL_init(); //
		ACL_set_sample_on();
		ACL_take_sample(sample); //pushes init
		usb_tx_string_P(PSTR("Accelerometer has been restarted\r"));
		break;
	case 's':  //sample rate
 		speed = atoi(CMD_buffer+4);
		if (speed > 3199) {
			ACL_set_rate(ACL_RATE_3200);	
			usb_tx_string_P(PSTR("Set to 3200Hz\r"));
		}else if (speed > 1599) {
			ACL_set_rate(ACL_RATE_1600);
			usb_tx_string_P(PSTR("Set to 1600Hz\r"));
		}else if (speed > 799) {
			ACL_set_rate(ACL_RATE_800);
			usb_tx_string_P(PSTR("Set to 800Hz\r"));
		}else if (speed > 399) {
			ACL_set_rate(ACL_RATE_400);
			usb_tx_string_P(PSTR("Set to 400Hz\r"));
		}else if (speed > 199) {
			ACL_set_rate(ACL_RATE_200);
			usb_tx_string_P(PSTR("Set to 200Hz\r"));
		}else if (speed > 99) {
			ACL_set_rate(ACL_RATE_100);
			usb_tx_string_P(PSTR("Set to 100Hz\r"));
		}else if (speed > 49) {
			ACL_set_rate(ACL_RATE_50);
			usb_tx_string_P(PSTR("Set to 50Hz\r"));
		}else if (speed > 24) {
			ACL_set_rate(ACL_RATE_25);
			usb_tx_string_P(PSTR("Set to 25Hz\r"));
		}else if (speed > 11) {
			ACL_set_rate(ACL_RATE_12);
			usb_tx_string_P(PSTR("Set to 12Hz\r"));
		}else if (speed > 5) {
			ACL_set_rate(ACL_RATE_6);
			usb_tx_string_P(PSTR("Set to 6Hz\r"));
		}else if (speed > 2) {
			ACL_set_rate(ACL_RATE_3);
			usb_tx_string_P(PSTR("Set to 3Hz\r"));
		}else if (speed > 0) {
			ACL_set_rate(ACL_RATE_1);
			usb_tx_string_P(PSTR("Set to 1Hz\r"));
 		}else {	
			usb_tx_string_P(PSTR("ERROR: Out of range, try again.\r"));	
		}
		break;
	case 'w':
		speed = atoi(CMD_buffer+4);
		if (speed > 16) {speed = 0;}
		if (speed > 8) {
			ACL_set_range(ACL_16G_RANGE);
			usb_tx_string_P(PSTR("+/-16G Range\r"));
		}else if (speed > 4) {
			ACL_set_range(ACL_8G_RANGE);
			usb_tx_string_P(PSTR("+/-8G Range\r"));
		}else if (speed > 2) {
			ACL_set_range(ACL_4G_RANGE);
			usb_tx_string_P(PSTR("+/-4G Range\r"));
		}else if (speed > 0) {
			ACL_set_range(ACL_2G_RANGE);
			usb_tx_string_P(PSTR("+/-2G Range\r"));
		}else {menu_send_out_of_range();}
		break;
		
	case 'x':
	case 'y':
	case 'z':
		data16 = atoi(CMD_buffer+4);  //convert input chars to int
		if (data16 == 0){ //conversion resulted in 0
			if (CMD_buffer[4] != '0'){ //check if it is really a '0' of an invalad conversion
				menu_send_out_of_range(); //invalid result
				break; //exit offset menu
			}
		}
		if (data16 < -127 || data16 > 127) {  //must be 16 to detect out of range
			menu_send_out_of_range();
		} else {
			uint8_t data8 = (uint8_t)data16;  //reduce to 8 bits of data
			switch (CMD_buffer[3]) {
				case 'x':
					usb_tx_string_P(PSTR("X"));
					ACL_set_offset(ACL_X_OFFSET, data8);
					break;
				case 'y':
					usb_tx_string_P(PSTR("Y"));
					ACL_set_offset(ACL_Y_OFFSET, data8);
					break;
				case 'z':
					usb_tx_string_P(PSTR("Z"));
					ACL_set_offset(ACL_Z_OFFSET, data8);
					break;
				default:
					menu_send_q();
					break;
			}
			usb_tx_string_P(PSTR(" Offset: "));
			menu_print_int(data16);
			menu_send_n();
		}
		break;
	case 'g':
		if (ACL_sampling()){
			if (usb_cdc_is_active(USB_ACL)) {
//				usb_tx_string_P(PSTR("Streaming, get last\r"));
				ACL_get_last_sample(sample);
			} else {
//				usb_tx_string_P(PSTR("Not streaming, get single\r"));
				ACL_take_sample(sample);
			}
			ACL_data_to_string(sample, CMD_buffer); 
			for (int i = 0; i < strlen(CMD_buffer); i++)
				{usb_cdc_send_byte(USB_CMD, CMD_buffer[i]);}
		} else {
			usb_tx_string_P(PSTR("ERROR: Not currently sampling, use VXAE\r"));}
		
		break;
	case 'f':
		if (CMD_buffer[4] == '0') {
			ACL_set_full_resolution(0);
			usb_tx_string_P(PSTR("10bit resolution, scale changes with 'G' range\r"));
		}
		else if (CMD_buffer[4] == '1') {
			ACL_set_full_resolution(1);
			usb_tx_string_P(PSTR("Full resolution, 4mg/bit\r"));
		}
		else {menu_send_q();}
		break;
	case '?':  
	default:
		usb_tx_string_P(PSTR("*** Accelerometer Menu ***\rE: Enable\rD: Disable\rR: Restart\rI: Subsystem Information\rQ: Query status\rG: Get last Sample\rSn: Set sample rate in HZ (1, 3, 6, 12, 25, 50, 100, 200, 400, 800)\rWn: Set \"G\" Range (2, 4, 8, 16)\rF: Full range (1: full 0: 10bit)\rXn: Set X offset (+/-127)\rYn: Set Y offset\rZn: Set Z offset\r"));
		break;
	}
}

void menu_modem (void) {
	int i; 
	switch (CMD_buffer[3]) {
	case 'd':  //disable
		power_sim_reset();
		usb_tx_string_P(PSTR("Modem is off\r"));
		break;
	case 'e':  //enable modem
		power_sim_start();
		usb_tx_string_P(PSTR("Modem has been started\r"));
		break;
	case 'r':  //reset
		power_sim_reset();
		power_sim_start();
		usb_tx_string_P(PSTR("Modem has been restarted\r"));
		break;
	case 'q':
		menu_modem_status();
		//simcard_status();
		break;
	case 'i':
		usb_tx_string_P(PSTR("V2X uses the SIM5320A 3G GSM modem + GPS receiver by SIMCOM\r"));
		break;
	case 'x':
		i = 4; //vxmx....
		usb_tx_string_P(PSTR(">>>GSM:"));
		usb_cdc_send_string(USB_CMD, CMD_buffer+4);
		while (CMD_buffer [i] != '\0') { //copy to output buffer
			GSM_add_to_buffer(BUFFER_OUT, CMD_buffer[i++]);
		}
 		GSM_add_to_buffer(BUFFER_OUT, '\r');//0x0D);
 		GSM_add_to_buffer(BUFFER_OUT, '\n');//0x0A);
		GSM_process_buffer(BUFFER_OUT);
		break;
	case '?':
	default:
		usb_tx_string_P(PSTR("*** Modem Menu ***\rE: Enable\rD: Disable\rR: Restart\rI: Subsystem Information\rQ: Query status\rX: AT Command Pass through\r"));
		break;
	}
}

void menu_can (void) {
	int i;
	switch (CMD_buffer[3]) {
	case 'd':  //disable
		power_control_turn_off((1<<ENABLE_CAN_RESET));
		power_control_push();
		usb_tx_string_P(PSTR("CANbus is off\r"));
		break;
	case 'e':  //enable
		power_control_turn_on((1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP));
		power_control_push();
		usb_tx_string_P(PSTR("CANbus has been started\r"));
		break;
	case 'r':  //reset
		power_control_turn_off((1<<ENABLE_CAN_RESET));
		power_control_push();
		delay_us(10);
		power_control_turn_on((1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP));
		power_control_push();
		usb_tx_string_P(PSTR("CANbus has been restarted\r"));
		
		break;
	case 'q':
		menu_can_status();
		break;
	case 'i':
		usb_tx_string_P(PSTR("V2X uses the STN1110 CANbus interface from Scantool\r"));
		usb_tx_string_P(PSTR("The STN1110 is compliant with the ELM327 V1.3\r"));
		break;
	case 'x':
		i = 4; //vxmx....
		usb_tx_string_P(PSTR(">>>CAN:"));
		usb_cdc_send_string(USB_CMD, CMD_buffer+4);
		while (CMD_buffer [i] != '\0') { //copy to output buffer
			CAN_add_to_buffer(BUFFER_OUT, CMD_buffer[i++]);
		}
		CAN_add_to_buffer(BUFFER_OUT, '\r');//0x0D);
//		CAN_add_to_buffer(BUFFER_OUT, '\n');//0x0A);
		CAN_process_buffer(BUFFER_OUT);
		break;
	case '?':
	default:
		usb_tx_string_P(PSTR("*** CANbus Menu ***\rE: Enable\rD: Disable\rR: Restart\rI: Subsystem Information\rQ: Query status\r"));
		break;
	}
}
void menu_power (void) {
	switch (CMD_buffer[3]) {
	case 'd':  //disable power
		switch (CMD_buffer[4]) {
		case '3':  //3v
			menu_send_ok();
			usb_tx_string_P(PSTR("Shutting down V2X in 30 seconds\r"));
			delay_s(30);
			power_control_turn_off((1<<ENABLE_3V3));
			power_control_push();
			break;
		case '4':  //4v
			power_control_turn_off((1<<ENABLE_4V1));
			power_control_push();
			menu_send_ok();
			break;
		case '5':  //5v
			power_control_turn_off((1<<ENABLE_5V0));
			power_control_push();
			menu_send_ok();
			break;
		case 'h':  //host
			power_control_turn_off((1<<ENABLE_5V0B));
			power_control_push();
			menu_send_ok();
			break;
		default:
			menu_send_q();
			break;
		}
		break; //disable power
	case 'e':  //enable power
		switch (CMD_buffer[4]) {
		case '3':  //3v
			power_control_turn_on((1<<ENABLE_3V3));
			power_control_push();
			menu_send_ok();
			break;
		case '4':  //4v
			power_control_turn_on((1<<ENABLE_4V1));
			power_control_push();
			menu_send_ok();
			break;
		case '5':  //5v
			power_control_turn_on((1<<ENABLE_5V0));
			power_control_push();
			menu_send_ok();
			break;
		case 'h':  //host
			power_control_turn_on((1<<ENABLE_5V0B));
			power_control_push();
			menu_send_ok();
			break;
		default:
			menu_send_q();
			break;
		}
		break;  //enable power
	case 'r':  
		//power_control_init;
		power_control_turn_off(~POWER_CONTROL_DEFAULT_VALUE);
		power_control_turn_on(POWER_CONTROL_DEFAULT_VALUE);
		power_control_push();
		menu_send_ok();
		break;
	case 'q':
		menu_power_status();
		break;
	case '?':
	default:
		usb_tx_string_P(PSTR("*** Power Menu ***\rEn: Enable power supply (3, 4, 5, H)\rDn: Disable power supply (3, 4, 5, H)\rR: Reset to defaults\rQ: Query status\r"));
		break;
	}
}

void menu_timer(void) {
	long data32;
	switch (CMD_buffer[3]) {
	case 's':  //set V2X time
		time_set(menu_sample_number(CMD_buffer+4));
		usb_tx_string_P(PSTR("Time has been set\r"));
		break;
	case 'g':  //get V2X time
		usb_tx_string_P(PSTR("UET: "));
		menu_print_int(time_get());
		usb_tx_string_P(PSTR("\rTime: "));
		print_human_time();
		menu_send_n();
		break;
	case 'a':  //absolute alarm
		time_alarm_set(menu_sample_number(CMD_buffer+4));
		usb_tx_string_P(PSTR("Alarm has been set"));
		break;
	case 'r':  //relative alarm
		time_alarm_set_relative(menu_sample_number(CMD_buffer+4));
		usb_tx_string_P(PSTR("Alarm has been set"));
		break;
	case 'z':  //set time zone
		time_zone_set(menu_sample_number(CMD_buffer+4));
		usb_tx_string_P(PSTR("Time Zone has been set"));
		break;
	case 'i':  //timer system information
		usb_tx_string_P(PSTR("The timer module uses Unix Epoch timestamps (UET) \rH24: clock has been set/sync within 24hrs\rALM: alarm is set for the future\r"));
		break;
	case 'q':  //timer inquery
		menu_timer_status();
		break;
	case '?':  //Menu options
	default:
		usb_tx_string_P(PSTR("*** Timer Menu ***\rSn: Set V2X time (UET)\rG: Get V2X time\rAn: Set absolute alarm (UET) \rRn: Set relative alarm (Seconds)\rI: timer information\rQ: Timer inquery\r"));
		break;
	}
}

void clear_buffer(char * buffer) {
	int cnt = strlen(buffer);
	for(int i = 0; i < cnt; i++)
		{buffer[i] = '\0';}
}
void menu_send_ok(void) {usb_tx_string_P(PSTR("OK\r"));
}
void menu_send_q(void) {usb_tx_string_P(PSTR("?\r"));
}
void menu_send_1(void) {usb_tx_string_P(PSTR("1\r"));
}
void menu_send_0(void) {usb_tx_string_P(PSTR("0\r"));
}
void menu_send_n(void) {usb_tx_string_P(PSTR("\r"));
}			
void menu_send_out_of_range(void) {	usb_tx_string_P(PSTR("ERROR: out of range\r"));
}
void menu_print_int(long value) {	
	char c_buf[13];
	ltoa(value, c_buf, 10);
	int i = 0;  //clear the pointer
	while (c_buf[i] != 0)
		{usb_cdc_send_byte(USB_CMD, c_buf[i++]);}
}

long menu_sample_number(char * input) {
	return atol(input);  //convert input chars to int
}

void usb_tx_string_P(const char *data) {
	while (pgm_read_byte(data))
		usb_cdc_send_byte(USB_CMD, pgm_read_byte(data++));
}

void menu_status (void) {
	menu_power_status();
	menu_accel_status();
	menu_can_status();
	menu_modem_status();
	menu_timer_status();
}
void menu_accel_status(void) {
	usb_tx_string_P(PSTR("ACL="));
	if (ACL_sampling())
	{menu_send_1();}
	else	{menu_send_0();}
}
void menu_modem_status(void) {
	usb_tx_string_P(PSTR("SIMPWR="));
	if (sim_power_status())
	{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("SIMNET="));
	if (sim_net_status())
	{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("SIMCARD="));
	if (simcard_status())
	{menu_send_1();}
	else	{menu_send_0();}
}
void menu_simcard_status(void){
	if (simcard_status())
		{menu_send_1();}
	else	{menu_send_0();}
}
void menu_can_status(void) {
	usb_tx_string_P(PSTR("CAN="));
	if (power_query((1<<ENABLE_CAN_RESET)))
			{menu_send_1();}
	else	{menu_send_0();}
}
void menu_power_status(void) {
	usb_tx_string_P(PSTR("3V3="));
	if (power_query((1<<ENABLE_3V3)))
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("4V1="));
	if (power_query((1<<ENABLE_4V1)))
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("5V0="));
	if (power_query((1<<ENABLE_5V0)))
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("USB="));
	if (power_query((1<<ENABLE_5V0B)))
			{menu_send_1();}
	else	{menu_send_0();}
}

void menu_timer_status (void) {
	usb_tx_string_P(PSTR("H24="));
	if (time_is_current())
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("ALM="));
	if (time_alarm_active())
			{menu_send_1();}
	else	{menu_send_0();}
	usb_tx_string_P(PSTR("TZN="));
	menu_print_int(time_zone_get());
	menu_send_n();
	usb_tx_string_P(PSTR("UET="));
	menu_print_int(time_get());		
	menu_send_n();
}
/*
 * V2X_menu.c
 *
 * Created: 2/23/2016 9:00:14 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"
#include "ctype.h"

char CMD_buffer [30] = "\0";
// const char JAG_JUMP1[] PROGMEM = "   ,ggp@@@@mgg,,\r,$Q$(`S@@$;g$$$$$@$@@gg,\r;gP'$@gg)$$@@$@@@$(L$$||$$@g,\r  `g$P`  ``*%@@@P`)Eg|||lLLL||$Bgg,\r    `       ,gg$$@gg,`$..``$Z$$$$$EB$$@g,\r         @P`pgg$$$||`)gggg;,,     |$$$|$$$@g,\r         9w&    '*^^` ``*P#9BB00000$$$@|`$$$g|Sg,\r                                    *$@@L ```T$W~)%g,\r                                      *%@gg,,,,,    5/Sw,     ,\r                                          ```` ` `9g `9g,``*^|'\r                                                    `#g,`)h\r\r   Developed at Jaguar Land Rover OSCT. Portland OR 2016\r";
uint8_t sample[6] = {0,0,0,0,0,0};

void menu_add_to_command(char value) {
	if (value == 8 | value == 0x7f) {				//if backspace or delete
		CMD_buffer[strlen(CMD_buffer)-1] = '\0';	//null out the deleted char
	} else if (value > 0x20 && value < 0x7f) {		//if in ascii set
		strcat(CMD_buffer, &value);					//store to buffer
	}
// 	for (int i = 0; i < strlen(CMD_buffer); i++)
// 	{usb_cdc_send_byte(USB_CMD, CMD_buffer[i]);}
// 	menu_send_n();
}

void menu_main(void) {
	int cnt = strlen(CMD_buffer);
	for(int i = 0; i < cnt; i++){
		CMD_buffer[i] = tolower(CMD_buffer[i]);
	}
	if (CMD_buffer[0] == 'v' && CMD_buffer[1] == 'x') {
		switch(CMD_buffer[2]) {
			case 'i': //information
				usb_tx_string_P(PSTR("Vehicle to Everything (V2X) RVI Node\rOpen source hardware and software\rHW Rev1.2 \rSW Rev0.1\r"));
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
				usb_tx_string_P(PSTR("Timer functions not implemented yet.\r"));
				break;
			case '?':  //timer functions
			default:
				usb_tx_string_P(PSTR("*** Main commands ***\rI: Device information\rA: Accelerometer\rC: CANbus\rM: Modem\rP: Power\rT: Timer\rQ: Query system status\rS: Simcard status\rR: Reboot\r"));
				break;
			}
	}else{
			usb_tx_string_P(PSTR("All commands start VX, VX? for commands\r"));
	}
	clear_CMD_buffer(CMD_buffer);	//clear the buffer for next command	
	usb_tx_string_P(PSTR("\r>"));  //prompt for user input
}

void menu_accel (void) {
	int speed, data16;
	uint8_t data8, i;
	char c_buf[6];
	switch (CMD_buffer[3]) {
	case 'd':  //disable
		ACL_sample_off();
		usb_tx_string_P(PSTR("Accelerometer is off\r"));
		break;
	case 'e':  //enable
		ACL_sample_on();
		usb_tx_string_P(PSTR("Accelerometer has been started\r"));
		break;
	case 'i':
		usb_tx_string_P(PSTR("V2X uses the ADXL345 accelerometer from Analog Devices\r"));
		break;
	case 'q':
		menu_accel_status();
		break;
	case 'r':  //reset
		ACL_sample_off();
		ACL_init();
		ACL_sample_on();
		usb_tx_string_P(PSTR("Accelerometer has been restarted\r"));
		menu_send_ok();
		break;
	case 's':  //sample rate
		data8 = 0; //clear for no-change detection at end
		speed = atoi(CMD_buffer+4);
		if (speed > 3199) {
			data8 = ACL_RATE_3200;	
			usb_tx_string_P(PSTR("Set to 3200Hz\r"));
		}else if (speed > 1599) {
			data8 = ACL_RATE_3200;	
			usb_tx_string_P(PSTR("Set to 1600Hz\r"));
		}else if (speed > 799) {
			data8 = ACL_RATE_1600;	
			usb_tx_string_P(PSTR("Set to 800Hz\r"));
		}else if (speed > 399) {
			 data8 = ACL_RATE_400;	
			usb_tx_string_P(PSTR("Set to 400Hz\r"));
		}else if (speed > 199) {
			 data8 = ACL_RATE_200;	
			usb_tx_string_P(PSTR("Set to 200Hz\r"));
		}else if (speed > 99) {
			 data8 = ACL_RATE_100;	
			usb_tx_string_P(PSTR("Set to 100Hz\r"));
		}else if (speed > 49) {
			 data8 = ACL_RATE_50;	
			usb_tx_string_P(PSTR("Set to 50Hz\r"));
		}else if (speed > 24) {
			 data8 = ACL_RATE_25;	
			usb_tx_string_P(PSTR("Set to 25Hz\r"));
		}else if (speed > 11) {
			 data8 = ACL_RATE_12;	
			usb_tx_string_P(PSTR("Set to 12Hz\r"));
		}else if (speed > 5) {
			 data8 = ACL_RATE_6;		
			usb_tx_string_P(PSTR("Set to 6Hz\r"));
		}else if (speed > 2) {
			 data8 = ACL_RATE_3;		
			usb_tx_string_P(PSTR("Set to 3Hz\r"));
		}else if (speed > 0) {
			 data8 = ACL_RATE_1;		
			usb_tx_string_P(PSTR("Set to 1Hz\r"));
		}else {	usb_tx_string_P(PSTR("ERROR: Does not compute, try again\r"));	}
		int ss_status = ioport_get_pin_level(ACL_MASTER_SS);
		if (ss_status == 0) 	{usb_tx_string_P(PSTR("ERROR: setting interrupted, try again\r"));}
		if (ss_status != 0 && data8 != 0) {
			int hold_state = ACL_sampling();
			ACL_sample_off();
			ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_BW_RATE), &data8, 2);
			if (hold_state) 
				{ACL_sample_on();}
			menu_send_ok();
		}
		break;
	case 'w':
		speed = atoi(CMD_buffer+4);
		ACL_send_recv_data(ACL_command_builder(ACL_READ, ACL_SINGLE, ACL_MAP_DATA_FORMAT), &sample, 2);
		sample[0] &= ~((1<<ACL_RANGE_L)|(1<<ACL_RANGE_H));
		if (speed > 8 && speed < 17) {
			sample[0] = (1<<ACL_RANGE_L)|(1<<ACL_RANGE_H); //full resolution 16g mode
			ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_DATA_FORMAT), &sample, 2);
			usb_tx_string_P(PSTR("+/-16G Range\r"));
		}else if (speed > 4) {
			sample[0] = (0<<ACL_RANGE_L)|(1<<ACL_RANGE_H); //full resolution 8g mode
			ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_DATA_FORMAT), &sample, 2);
			usb_tx_string_P(PSTR("+/-8G Range\r"));
		}else if (speed > 2) {
			sample[0] = (1<<ACL_RANGE_L)|(0<<ACL_RANGE_H); //full resolution 4g mode
			ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_DATA_FORMAT), &sample, 2);
			usb_tx_string_P(PSTR("+/-4G Range\r"));
		}else if (speed > 0) {
			sample[0] = (0<<ACL_RANGE_L)|(0<<ACL_RANGE_H); //full resolution 2g mode
			ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_DATA_FORMAT), &sample, 2);
			usb_tx_string_P(PSTR("+/-2G Range\r"));
		}else {menu_send_q();}
		break;
	case 'x':
		data16 = atoi(CMD_buffer+4);
		if (data16 < -127 || data16 > 127) {  //must be 16 to detect out of range
			usb_tx_string_P(PSTR("ERROR: out of range, +-127\r"));
		} else {
			data8 = (uint8_t)data16;  //reduce to 8 bits of data
			usb_tx_string_P(PSTR("X Offset: "));
			itoa(data16, c_buf, 10);
			i = 0;  //clear the pointer
			while (c_buf[i] != 0)
				{usb_cdc_send_byte(USB_CMD, c_buf[i++]);}
			menu_send_n();
			ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_OFSX), &data8, 2);
		}
		break;
	case 'y':
		data16 = atoi(CMD_buffer+4);
		if (data16 < -127 || data16 > 127) {  //must be 16 to detect out of range
			usb_tx_string_P(PSTR("ERROR: out of range, +-127\r"));
			} else {
			data8 = (uint8_t)data16;  //reduce to 8 bits of data
			usb_tx_string_P(PSTR("Y Offset: "));
			itoa(data16, c_buf, 10);
			i = 0;  //clear the pointer
			while (c_buf[i] != 0)
			{usb_cdc_send_byte(USB_CMD, c_buf[i++]);}
			menu_send_n();
			ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_OFSY), &data8, 2);
		}
		break;
	case 'z':
		data16 = atoi(CMD_buffer+4);
		if (data16 < -127 || data16 > 127) {  //must be 16 to detect out of range
			usb_tx_string_P(PSTR("ERROR: out of range, +-127\r"));
			} else {
			data8 = (uint8_t)data16;  //reduce to 8 bits of data
			usb_tx_string_P(PSTR("Z Offset: "));
			itoa(data16, c_buf, 10);
			i = 0;  //clear the pointer
			while (c_buf[i] != 0)
			{usb_cdc_send_byte(USB_CMD, c_buf[i++]);}
			menu_send_n();
			ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_OFSZ), &data8, 2);
		}
		break;
	case 'g':
		if (ACL_sampling() && usb_cdc_is_active(USB_ACL)) {
			ACL_get_last_sample(sample);
			ACL_data_to_string(sample, CMD_buffer); 
			for (int i = 0; i < strlen(CMD_buffer); i++)
				{usb_cdc_send_byte(USB_CMD, CMD_buffer[i]);}
		} else {
			usb_tx_string_P(PSTR("ERROR: Not currently sampling\r"));}
		
		break;
	case '?':  
	default:
		usb_tx_string_P(PSTR("*** Accelerometer Menu ***\rE: Enable\rD: Disable\rR: Restart\rI: Subsystem Information\rQ: Query status\rG: Get last Sample\rSn: Set sample rate in HZ (1, 3, 6, 12, 25, 50, 100, 200, 400, 800)\rWn: Set \"G\" Range (2, 4, 8, 16)\rXn: Set X offset (+/-127)\rYn: Set Y offset\rZn: Set Z offset\r"));
		break;
	}
}

void menu_modem (void) {
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
	case '?':
	default:
		usb_tx_string_P(PSTR("*** Modem Commands ***\rE: Enable\rD: Disable\rR: Restart\rI: Subsystem Information\rQ: Query status\r"));
		break;
	}
}

void menu_can (void) {
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
			power_control_turn_off((1<<ENABLE_3V3));
			power_control_push();
			menu_send_ok();
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

void clear_CMD_buffer(char * CMD_buffer) {
	int cnt = strlen(CMD_buffer);
	for(int i = 0; i < cnt; i++)
		{CMD_buffer[i] = '\0';}
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

void usb_tx_string_P(const char *data) {
	while (pgm_read_byte(data))
		usb_cdc_send_byte(USB_CMD, pgm_read_byte(data++));
}

void menu_status (void) {
	menu_power_status();
	menu_accel_status();
	menu_can_status();
	menu_modem_status();
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
	usb_tx_string_P(PSTR("HOST="));
	if (power_query((1<<ENABLE_5V0B)))
			{menu_send_1();}
	else	{menu_send_0();}

}

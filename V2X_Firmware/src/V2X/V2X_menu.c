/*
 * V2X_menu.c
 *
 * Created: 2/23/2016 9:00:14 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"
#include "ctype.h"

char CMD_buffer [30] = "\0";

void menu_add_to_command(char value) {
	if (value == 8 | value == 0x7f) {				//if backspace or delete
		CMD_buffer[strlen(CMD_buffer)-1] = '\0';	//null out the deleted char
	} else if (value > 0x20 && value < 0x7f) {		//if in ascii set
		strcat(CMD_buffer, &value);					//store to buffer
	}
}

void menu_parse_command(void) {
	int cnt = strlen(CMD_buffer);
	for(int i = 0; i < cnt; i++){
		CMD_buffer[i] = tolower(CMD_buffer[i]);
	}
	if (CMD_buffer[0] == 'v' && CMD_buffer[1] == 'x') {
		switch(CMD_buffer[2]) {
			case 'i': //information
				usb_cdc_send_string(USB_CMD, "Device ID: V2X Rev1.2\r");
				break;
			case 'z':  //reset
				usb_cdc_send_string(USB_CMD, "Reset function not available yet, please power cycle.\r");
				break;
			case 'a':  //accel
				menu_accel();
				break;
			case 'c':  //CAN interface
				menu_can();
				break;
			case 'm':  //modem
				menu_modem();
				break;
			case 'p':  //power
				menu_power();
				break;
			case 't':  //timer functions
				usb_cdc_send_string(USB_CMD, "Timer functions not active yet.\r");
				break;
			case '?':  //timer functions
				usb_cdc_send_string(USB_CMD, "Main menu\r");
				usb_cdc_send_string(USB_CMD, "I: Device information\r");
				usb_cdc_send_string(USB_CMD, "A: Accelerometer\r");
				usb_cdc_send_string(USB_CMD, "C: CANBUS\r");
				usb_cdc_send_string(USB_CMD, "M: Modem\r");
				usb_cdc_send_string(USB_CMD, "P: Power\r");
				usb_cdc_send_string(USB_CMD, "T: Timer\r");
				usb_cdc_send_string(USB_CMD, "Z: Reboot\r");
				break;
			default:
				menu_send_q();
			}
	}else{
			usb_cdc_send_string(USB_CMD, "Commands start VX, ? for options.\r");
	}

	//clear the buffer for next command	
	for(int i = 0; i < cnt; i++){
		CMD_buffer[i] = '\0';
	}
	usb_cdc_send_string(USB_CMD, ">");
}

void menu_send_ok(void) {usb_cdc_send_string(USB_CMD, "OK\r");
}

void menu_send_q(void) {usb_cdc_send_string(USB_CMD, "?\r");
}

void menu_send_1(void) {usb_cdc_send_string(USB_CMD, "1\r");
}

void menu_send_0(void) {usb_cdc_send_string(USB_CMD, "0\r");
}

void menu_accel (void){
	switch (CMD_buffer[3]) {
		case 'd':  //disable
			ACL_sample_off();
			break;
		case 'e':  //enable
			ACL_sample_on();
			break;
		case 'r':  //reset
			ACL_sample_off();
			ACL_init();
			break;
		case 's':  //sample rate
			//needs a number detector
			break;
		case 'x':
			break;
		case 'y':
			break;
		case 'z':
			break;
		case '?':  
			usb_cdc_send_string(USB_CMD, "E: Enable\r");
			usb_cdc_send_string(USB_CMD, "D: Disable\r");
			usb_cdc_send_string(USB_CMD, "R: Reset\r");
			usb_cdc_send_string(USB_CMD, "Sn: Sample rate (1, 3, 6, 12, 25, 50, 100, 200, 400, 800)\r");
			usb_cdc_send_string(USB_CMD, "Rn: Range (2, 4, 8, 16G)\r");
			usb_cdc_send_string(USB_CMD, "Xn: X offset (-2048 to 2048)\r");
			usb_cdc_send_string(USB_CMD, "Xn: Y offset\r");
			usb_cdc_send_string(USB_CMD, "Xn: Z offset\r");
			break;
		default:
			menu_send_q();
			break;
	}
}

void menu_modem (void) {
	switch (CMD_buffer[3]) {
	case 'd':  //disable
		power_sim_reset();
		menu_send_ok();
		break;
	case 'e':  //enable modem
		power_sim_start();
		menu_send_ok();
		break;
	case 'r':  //reset
		power_sim_reset();
		power_sim_start();
		menu_send_ok();
		break;
	case 'i':
		usb_cdc_send_string(USB_CMD, "SIMPWR=");
		if (sim_power_status())
				{menu_send_1();}
		else	{menu_send_0();}
		usb_cdc_send_string(USB_CMD, "SIMNET=");
		if (sim_net_status()) 
				{menu_send_1();}
		else	{menu_send_0();}
		usb_cdc_send_string(USB_CMD, "SIMCARD=");
		if (simcard_status()) 
				{menu_send_1();}
		else	{menu_send_0();}
		break;
	default:
		menu_send_q();
		break;	
	}
}

void menu_can (void) {
	switch (CMD_buffer[3]) {
	case 'd':  //disable
		power_control_turn_on((1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP));
		power_control_push();
		menu_send_ok();
		break;
	case 'e':  //enable
		power_control_turn_off((1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP));
		power_control_push();
		menu_send_ok();
		break;
	case 'r':  //reset
		power_control_turn_on((1<<ENABLE_CAN_RESET));
		power_control_push();
		delay_ms(50);
		power_control_turn_off((1<<ENABLE_CAN_RESET));
		power_control_push();
		menu_send_ok();
		break;
	default:
		menu_send_q();
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
	case 'i':
		usb_cdc_send_string(USB_CMD, "3V3=");
		if (power_query((1<<ENABLE_3V3)))
		{menu_send_1();}
		else	{menu_send_0();}
		usb_cdc_send_string(USB_CMD, "4V1=");
		if (power_query((1<<ENABLE_4V1)))
		{menu_send_1();}
		else	{menu_send_0();}
		usb_cdc_send_string(USB_CMD, "5V0=");
		if (power_query((1<<ENABLE_5V0)))
		{menu_send_1();}
		else	{menu_send_0();}
		usb_cdc_send_string(USB_CMD, "HOST=");
		if (power_query((1<<ENABLE_5V0B)))
		{menu_send_1();}
		else	{menu_send_0();}
		break;
		default:
		menu_send_q();
		break;
	}
}
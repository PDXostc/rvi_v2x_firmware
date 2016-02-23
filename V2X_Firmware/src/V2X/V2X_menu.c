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
// 	int cnt = strlen(CMD_buffer);
// 	CMD_buffer[cnt++] = value;
// 	CMD_buffer[cnt] = '\0';
	strcat(CMD_buffer, &value);
}

void menu_parse_command(void) {
	int cnt = strlen(CMD_buffer);
	for(int i = 0; i < cnt; i++){
		CMD_buffer[i] = tolower(CMD_buffer[i]);
	}
	if (CMD_buffer[0] == 'v' && CMD_buffer[1] == 'x') {
		usb_cdc_send_string(USB_CMD, "OK\r");
	}else{
		usb_cdc_send_string(USB_CMD, "?\r");
	}
	//CMD_buffer = "\0"; //clear the buffer for next time
	//memset(CMD_buffer, '\0', cnt+1);
	for(int i = 0; i < cnt; i++){
		CMD_buffer[i] = '\0';
	}
	
}

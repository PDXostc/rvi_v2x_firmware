/*
 * V2X_menu.c
 *
 * Created: 2/23/2016 9:00:14 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

char CMD_buffer [30] = "\0";

void menu_add_to_command(char value) {
// 	int cnt = strlen(CMD_buffer);
// 	CMD_buffer[cnt++] = value;
// 	CMD_buffer[cnt] = '\0';
	strcat(CMD_buffer, value);
}

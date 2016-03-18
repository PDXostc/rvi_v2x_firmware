/*
 * V2X_time.c
 *
 * Created: 3/2/2016 3:24:15 PM
 *  Author: jbanks2
 */ 

#include "V2X.h"

Bool alarm_is_set = false;
Bool time_is_set = false; 
long time_was_set = 0; //marks the last time the clock was adjusted
int time_zone = -8;

struct calendar_date date_s = {
	.second = 12,
	.minute = 1,
	.hour = 22,
	.date = 8,
	.month = 2,
	.year = 1985
};

void time_init(void) {
	rtc_init();
	rtc_set_time(time_seed);
	rtc_set_callback(time_alarm_event);
	//read timezone stored in EEPROM
	
}

void time_set(long time) {
	rtc_set_time(time);
	time_was_set = time;
	time_is_set = true;
}

long time_get(void) {
	time_24hr_check();
	return  rtc_get_time();
}

void time_alarm_set(long time) {
	alarm_is_set = true;
	rtc_set_alarm(time);
}

void time_alarm_set_relative(long offset) {
	alarm_is_set = true;
	rtc_set_alarm_relative(offset);
}

void time_alarm_event(void) {
	alarm_is_set = false;
	usb_tx_string_P(PSTR("ALARM\r\r>"));
	//respond to alarm
	//set flag that time alarm caused wakeup
}

Bool time_alarm_active(void) {
	return alarm_is_set;
}

Bool time_is_current(void) {
	time_24hr_check();
	return time_is_set;
}

void time_24hr_check (void){
	if (rtc_get_time() - time_was_set > 86400) {time_is_set = false;}
}

void time_zone_set (int zone) {
	time_zone = zone;
}

int time_zone_get (void) {
	return time_zone;
}

void print_human_time (void) {
	calendar_timestamp_to_date_tz(rtc_get_time(),time_zone,0,&date_s);
	menu_print_int(date_s.month+1);
	usb_tx_string_P(PSTR("/"));
	menu_print_int(date_s.date+1);
	usb_tx_string_P(PSTR("/"));
	menu_print_int(date_s.year);
	usb_tx_string_P(PSTR(" "));
	menu_print_int(date_s.hour);
	usb_tx_string_P(PSTR(":"));
	menu_print_int(date_s.minute);
	usb_tx_string_P(PSTR(":"));
	menu_print_int(date_s.second);
}

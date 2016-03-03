/*
 * V2X_time.c
 *
 * Created: 3/2/2016 3:24:15 PM
 *  Author: jbanks2
 */ 

#include "V2X.h"

void time_init(void) {
	rtc_init();
	rtc_set_time(time_seed);
	rtc_set_callback(time_alarm_event);
}

void time_set(uint32_t time) {
	rtc_set_time(time);
}

uint32_t time_get(void) {
	return rtc_get_time();
}

void time_alarm_set(uint32_t time) {
	rtc_set_alarm(time);
}

void time_alarm_set_relative(uint32_t offset) {
		rtc_set_alarm_relative(offset);
}

void time_alarm_event() {
	//respond to alarm
	//set flag that time alarm caused wakeup
}

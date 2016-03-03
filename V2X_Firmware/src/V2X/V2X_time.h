/*
 * V2X_time.h
 *
 * Created: 3/2/2016 3:24:33 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_TIME_H_
#define V2X_TIME_H_

#define time_seed 1456961042  //time starter

/**
 * @def time_init
 * @brief sets up RTC for opperation
 **/
void time_init(void);

/**
 * @def time_set
 * @brief set the internal time of the RTC 
 * @param time unix epoch time stamp 
 **/
void time_set(uint32_t time);

/**
 * @def time_get
 * @brief reports the current time
 * @retval current system time as unix epoch time stamp
 **/
uint32_t time_get(void);

/**
 * @def time_alarm_set
 * @brief set an alarm to a specific unix epoch time stamp 
 * @param time unix epoch time stamp
 **/
void time_alarm_set(uint32_t time);

/**
 * @def time_alarm_set_relative
 * @brief sets alarm for a time in the future relative to current time 
 * @param offset time in seconds until alarm
 **/
void time_alarm_set_relative(uint32_t offset);

/**
 * @def time_alarm_event
 * @brief callback function for alarm events 
 **/
void time_alarm_event();

#endif /* V2X_TIME_H_ */
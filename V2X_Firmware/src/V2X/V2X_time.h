/*
 * V2X_time.h
 *
 * Created: 3/2/2016 3:24:33 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_TIME_H_
#define V2X_TIME_H_

#define time_seed 1456961042  //time starter
#define RTC_VALIDITY_PERIOD 600  //RTC syncs with GPS

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
void time_set(long time);

/**
 * @def time_get
 * @brief reports the current time. includes time_24hr_check
 * @retval current system time as unix epoch time stamp
 **/
long time_get(void);

/**
 * @def time_24hr_check
 * @brief clears time_is_current holder after 24hrs
 **/
void time_24hr_check (void);

/**
 * @def time_alarm_set
 * @brief set an alarm to a specific unix epoch time stamp 
 *	sets time_alarm_active holder.
 * @param time unix epoch time stamp
 **/
void time_alarm_set(long time);

/**
 * @def time_alarm_set_relative
 * @brief sets alarm for a time in the future relative to current time 
 *	sets time_alarm_active holder.
 * @param offset time in seconds until alarm
 **/
void time_alarm_set_relative(long offset);

/**
 * @def time_alarm_event
 * @brief callback function for alarm events, 
 *	clears time_alarm_active holder
 **/
void time_alarm_event(void);

/**
 * @def time_alarm_active
 * @brief query if alarm was set, and not yet gone off 
 * @retval true if alarm is currently set for a time in the future
 **/
Bool time_alarm_active(void);

/**
 * @def time_is_current
 * @brief includes time_24hr_check, reports if time was set in the last 24 hours
 * @retval true if time is currently considered accurate
 **/
Bool time_is_current(void);

/**
 * @def time_zone_set
 * @brief set the time zone used by calender and RTC 
 * @param zone UTC index for time zone (+16 to -12)
 **/
void time_zone_set (int zone);

/**
 * @def time_zone_set
 * @brief set the time zone used by calender and RTC 
 * @retval UTC index for time zone (+16 to -12)
 **/
int time_zone_get (void);

/**
 * @def time_print_human_readable
 * @brief prints a human readable timestamp to the CMD interface 
 **/
void time_print_human_readable (void);

/**
 * @def time_set_by_strings
 * @brief set the time zone used by calender and RTC 
 * @param *date string of chars containing date info DDMMYY
 * @param *time string of chars containing date info HHMMSS
 **/
void time_set_by_strings (char * date, char * time);

#endif /* V2X_TIME_H_ */
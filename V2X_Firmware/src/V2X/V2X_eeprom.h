/*
 * V2X_eeprom.h
 *
 * Created: 3/29/2016 8:14:04 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_EEPROM_H_
#define V2X_EEPROM_H_

#define	EE_CHECK_VALUE 0xA5
#define EE_CAN_ARRAY_SIZE 30

enum eeprom_storage_names {
	EE_timezone = 0,
	EE_dst,
	EE_verbose,
	EE_check_byte,
	EE_can_array //must be last
	};

/**
 * @def time_alarm_active
 * @brief checks value of EE_check_byte to know id EE has been init with default values
 *		if not correct, default values are saved then loaded
 **/
void eeprom_init (void);

/**
 * @def eeprom_store_CAN_string
 * @brief pass in a string and it will be stored to EEPROM
 **/
void eeprom_store_CAN_string (char * buffer);

/**
 * @def time_alarm_active
 * @brief checks value of EE_check_byte to know id EE has been init with default values
 *		if not correct, default values are saved then loaded
 **/
void eeprom_read_CAN_string (char * buffer);

#endif /* V2X_EEPROM_H_ */
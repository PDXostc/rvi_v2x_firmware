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
	EE_dst = 1,
	EE_verbose = 2,
	EE_car_state_check_enabled = 3,
	EE_car_state_check_low_power_check_interval = 4,
	EE_car_state_check_high_power_check_interval = 6, // skip two for low/high bytes of previous addr
	EE_check_byte = 8, // skip two for low/high bytes of previous addr
	EE_can_array = 9 //must be last
	};

/**
 * @def time_alarm_active
 * @brief checks value of EE_check_byte to know id EE has been init with default values
 *		if not correct, default values are saved then loaded
 **/
void eeprom_init (void);

/**
 * @def eeprom_read_int
 * @brief helper function for retrieving ints
 * @param addr of high byte
 * @return value stored in eeprom
 */
int eeprom_read_int(eeprom_addr_t addr);

/**
 * @def eeprom_write_int
 * @brief helper function for storing ints
 * @param addr of high byte
 * @param value int value to be stored
 */

void eeprom_write_int(eeprom_addr_t addr, int value);

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
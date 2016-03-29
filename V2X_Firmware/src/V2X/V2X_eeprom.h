/*
 * V2X_eeprom.h
 *
 * Created: 3/29/2016 8:14:04 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_EEPROM_H_
#define V2X_EEPROM_H_

enum eeprom_storage_names {
	EE_timezone = 0,
	EE_dst,
	EE_check_byte,
	EE_check_value = 0xA5
	};

/**
 * @def time_alarm_active
 * @brief checks value of EE_check_byte to know id EE has been init with default values
 *		if not correct, default values are saved then loaded
 **/
void eeprom_init (void);

#endif /* V2X_EEPROM_H_ */
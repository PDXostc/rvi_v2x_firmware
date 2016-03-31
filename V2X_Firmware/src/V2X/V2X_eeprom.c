/*
 * V2X_eeprom.c
 *
 * Created: 3/29/2016 8:19:45 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

void eeprom_init (void) {
	if ( nvm_eeprom_read_byte(EE_check_byte) != EE_check_value) {
		//the check byte was not found, set EE defaults
		nvm_eeprom_write_byte(EE_timezone, time_zone_get());
		nvm_eeprom_write_byte(EE_dst, time_dst_get());
		//set "eeprom is initilized" check byte
		nvm_eeprom_write_byte(EE_check_byte, EE_check_value);
	}
	
}
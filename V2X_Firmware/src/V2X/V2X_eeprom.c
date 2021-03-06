/*
 * V2X_eeprom.c
 *
 * Created: 3/29/2016 8:19:45 AM
 *  Author: Jesse Banks
 */ 

#include "V2X.h"

void eeprom_init (void) {
	if ( nvm_eeprom_read_byte(EE_check_byte) != EE_CHECK_VALUE) {
		//the check byte was not found, set EE defaults
		nvm_eeprom_write_byte(EE_timezone, time_zone_default_offset());  
		nvm_eeprom_write_byte(EE_dst, false);
		nvm_eeprom_write_byte(EE_verbose, false);
		nvm_eeprom_write_byte(EE_car_state_check_enabled, CSC_get_car_state_check_default_enabled());
        eeprom_write_unsigned_int_16(EE_car_state_check_low_power_check_interval,
                                     CSC_get_car_state_check_low_power_default_interval());
        eeprom_write_unsigned_int_16(EE_car_state_check_high_power_check_interval,
                                     CSC_get_car_state_check_high_power_default_interval());
		
		for (int i = 0; i < EE_CAN_ARRAY_SIZE; i++) {
			nvm_eeprom_write_byte(EE_can_array + i, '\0');  //add null at start
		}
		//set "eeprom is initilized" check byte
		nvm_eeprom_write_byte(EE_check_byte, EE_CHECK_VALUE);
	}
}

void eeprom_write_unsigned_int_16(eeprom_addr_t addr, uint16_t value) {

    uint8_t high = (value>>8);
    uint8_t low  = value & 0xff;

    nvm_eeprom_write_byte(addr, high);
    nvm_eeprom_write_byte(addr+1, low);
}

uint16_t eeprom_read_unsigned_int_16(eeprom_addr_t addr) {

    uint8_t high = nvm_eeprom_read_byte(addr);
    uint8_t low  = nvm_eeprom_read_byte(addr+1);

    return high << 8 | low;
}

void eeprom_store_CAN_string (char * buffer) {
	uint8_t i = 0;
	while (buffer[i] != '\0' && i < EE_CAN_ARRAY_SIZE - 1) {  //allow room for null char
		nvm_eeprom_write_byte(EE_can_array + i, buffer[i]);
		i++;
	}
	nvm_eeprom_write_byte(EE_can_array + i, '\0');  //add null
}

void eeprom_read_CAN_string (char * buffer) {
	clear_buffer(buffer);
	uint8_t i = 0;
	char value = '1';
	while (value != '\0' && i < EE_CAN_ARRAY_SIZE) {  //get chars into buffer including null terminator
		value = nvm_eeprom_read_byte(EE_can_array + i);
		buffer[i++] = value;
	}
}
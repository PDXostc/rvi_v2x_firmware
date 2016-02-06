/**
 * /file V2X_drivers.c
 *
 * /brief V2X board hardware driver functions
 *
 * Author: Jesse Banks (jbanks2)
 **/ 

#include "V2X_drivers.h"

void canbus_serial_routing(uint8_t source)
{
	gpio_set_pin_low(BUF0_PIN);
	gpio_set_pin_low(BUF1_PIN);
	if		(source == FTDI_ROUTING)	{gpio_set_pin_high(BUF1_PIN);}
	else  /*(source == AVR_ROUTING)*/	{gpio_set_pin_high(BUF0_PIN);}
}

void shift_register_init(void)
{
	shift_register_clear();							// Clear shift register
	shift_register_latch();							// Latch all internal registers to output
	shift_register_state = STATE_DEFAULT_VALUE;		// set power state to dafault
	state_to_shift_register();						// cause shift register to update
}

void shift_register_latch(void)
{
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
	gpio_set_pin_high(EXT1_PIN_SR_LATCH);	//latch values to output buffer
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
}

void shift_register_clear(void)
{
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
	gpio_set_pin_low(EXT1_PIN_SR_CLEAR);	//clear all internal registers
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
}

void state_to_shift_register(void) {
	uint16_t bits = shift_register_state;
	for (int i=0; i<16; i++) {					//soft SPI routine to send data
		if (bits>>i & 1)
		{gpio_set_pin_high(EXT1_PIN_SPI_MOSI);}
		else
		{gpio_set_pin_low(EXT1_PIN_SPI_MOSI);}
		
		gpio_set_pin_high(EXT1_PIN_SPI_SCK);
		gpio_set_pin_low(EXT1_PIN_SPI_SCK);
	}
	shift_register_latch();
}

void turn_on (uint16_t pins_mask) {  //updates teh power state variable but does not update shift register
	shift_register_state |= pins_mask;
}

void turn_off(uint16_t pins_mask) {  //updates teh power state variable but does not update shift register
	shift_register_state &= ~(pins_mask);
}
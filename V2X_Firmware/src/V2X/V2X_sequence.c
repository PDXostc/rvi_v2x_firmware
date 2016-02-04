/*
 * Sequence.c
 *
 * Created: 10/15/2015 2:45:54 PM
 *  Author: Spencer
 */ 

#include "V2X_board.h"
#include "V2X_sequence.h"

void shiftRegisterInit(void)
{
	state = STATE_DEFAULT_VALUE;
	shiftRegisterClear();
	shiftRegisterLatch();
	bits_To_Shift_Register(state);
}

void shiftRegisterLatch(void)
{
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);	
	gpio_set_pin_high(EXT1_PIN_SR_LATCH);	//latch cleared values to output buffer
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
}

void shiftRegisterClear(void)
{
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
	gpio_set_pin_low(EXT1_PIN_SR_CLEAR);	//clear all internal registers
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);	
}

void bits_To_Shift_Register(uint16_t bits) {
	
	state = bits;
	for (int i=0; i<16; i++) {
		
		if (bits>>i & 1)
		{gpio_set_pin_high(EXT1_PIN_SPI_MOSI);}
		else
		{gpio_set_pin_low(EXT1_PIN_SPI_MOSI);}
		
		gpio_set_pin_high(EXT1_PIN_SPI_SCK);
		gpio_set_pin_low(EXT1_PIN_SPI_SCK);
	}
	shiftRegisterLatch();
}

/*
void run_sequence(struct step sequence[], int size) {
	int i;
	
	for (i=0; i<size; i++) {
		delay_ms(sequence[i].interval);
		bits_To_Shift_Register(sequence[i].value);
	}
}
*/

void turn_on (uint16_t pin_on_mask) {
	state |= pin_on_mask; //| state;
	bits_To_Shift_Register(state);
}

void turn_off(uint16_t pin_off_mask)
{
	state &= ~(pin_off_mask); //& state;
	bits_To_Shift_Register(state);
}

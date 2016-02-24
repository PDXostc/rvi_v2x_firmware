/**
 * /file V2X_drivers.c
 *
 * /brief V2X board hardware driver functions
 *
 * Author: Jesse Banks (jbanks2)
 **/ 

#include "V2X.h"

void power_control_init(void)
{
	power_control_clear();							// Clear shift register
	power_control_latch();							// Latch all internal registers to output

	power_control_turn_on((0<<ENABLE_4V1)|(1<<ENABLE_3V3)|(0<<ENABLE_SIM_RESET)|(0<<ENABLE_5V0)); // turn on GSM device power, allow out of reset
	power_control_push();		//update shift register state
	delay_ms(200);				//allow power to stabilize

}

void power_control_latch(void)
{
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
	gpio_set_pin_high(EXT1_PIN_SR_LATCH);	//latch values to output buffer
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
}

void power_control_clear(void)
{
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
	gpio_set_pin_low(EXT1_PIN_SR_CLEAR);	//clear all internal registers
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
}

void power_control_push(void) {  //uses shift_register_state to update the shift register
	uint8_t data[2];
	data[1] = power_control_state & 0xff;
	data[0] = (power_control_state >> 8) & 0xff; 
	spi_write_packet(SR_SPI, data, 2);
	power_control_latch();
}

void power_control_turn_on (uint16_t pins_mask) {  //updates teh power state variable but does not update shift register
	power_control_state |= pins_mask;
}

void power_control_turn_off(uint16_t pins_mask) {  //updates teh power state variable but does not update shift register
	power_control_state &= ~(pins_mask);
}
void power_sim_reset(void) {
	power_control_turn_off((1<<ENABLE_SIM_RESET));
	power_control_push();
}

void power_sim_start(void) {
	delay_ms(10);							//let power come up
	power_control_turn_on((1<<ENABLE_SIM_RESET));  //release the reset
	power_control_push();
	delay_ms(100);							//let power come up
	power_control_turn_on((1<<ENABLE_SIM_PWR_ON));
	power_control_push();
	delay_ms(50);							//let chip detect
	power_control_turn_off((1<<ENABLE_SIM_PWR_ON));
	power_control_push();					//clear the start bit
}

bool power_query(uint16_t mask) {
	if ((power_control_state & mask) != 0) {return true;}
	else {return false;}
}
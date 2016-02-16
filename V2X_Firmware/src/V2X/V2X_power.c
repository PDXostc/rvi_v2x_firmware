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
// 	power_control_state = POWER_CONTROL_DEFAULT_VALUE;		// set power state to default
// 	power_control_push();						// cause shift register to update
// 	delay_ms(200);		//allow power to stabilize
 		
	power_control_turn_on((1<<ENABLE_4V1)|(1<<ENABLE_3V3)|(1<<ENABLE_SIM_RESET)|(1<<ENABLE_5V0)); // turn on GSM device power, allow out of reset
	power_control_push();  //update shift register state
	delay_ms(200);				//allow power to stabilize
		
	power_control_turn_on((1<<ENABLE_SIM_PWR_ON)|(1<<ENABLE_SIM_RF_OFF)|(1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP)); //beginning of power on pulse
	power_control_push();  //update shift register state
	delay_ms(100);				//allow power to stabilize
		
	power_control_turn_off((1<<ENABLE_SIM_PWR_ON)); //end of "on power" on pulse to gsm device
	power_control_push();  //update shift register state

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
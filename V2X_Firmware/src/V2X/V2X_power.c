/**
 * /file V2X_drivers.c
 *
 * /brief V2X board hardware driver functions
 *
 * Author: Jesse Banks (jbanks2)
 **/ 

#include "V2X.h"

void PWR_init(void)
{
	PWR_clear();							// Clear shift register
	PWR_latch();							// Latch all internal registers to output
	power_control_state = POWER_CONTROL_DEFAULT_VALUE;
	PWR_push();		//update shift register state
	delay_ms(100);				//allow power to stabilize
}

void PWR_latch(void)
{
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
	gpio_set_pin_high(EXT1_PIN_SR_LATCH);	//latch values to output buffer
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
}

void PWR_clear(void)
{
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
	gpio_set_pin_low(EXT1_PIN_SR_CLEAR);	//clear all internal registers
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
}

void PWR_push(void) {  //uses shift_register_state to update the shift register
	uint8_t data[2];
	data[1] = power_control_state & 0xff;
	data[0] = (power_control_state >> 8) & 0xff; 
	spi_write_packet(SR_SPI, data, 2);
	PWR_latch();
}

void PWR_turn_on (uint16_t pins_mask) {  //updates the power state variable but does not update shift register
	power_control_state |= pins_mask;
}

void PWR_turn_off(uint16_t pins_mask) {  //updates the power state variable but does not update shift register
	power_control_state &= ~(pins_mask);
}

Bool PWR_query(uint16_t mask) {
	if ((power_control_state & mask) != 0) {return true;}
	else {return false;}
}

void PWR_hub_start(void) {
	PWR_turn_on((1<<ENABLE_HUB));
	PWR_push();
}

void PWR_hub_stop(void){
	PWR_turn_off((1<<ENABLE_HUB));
	PWR_push();
}

void PWR_host_start(void) {
	PWR_turn_on((1<<ENABLE_5V0B));
	PWR_is_5_needed();
}

void PWR_host_stop(void){
	PWR_turn_off((1<<ENABLE_5V0B));
	PWR_is_5_needed();
}

void PWR_is_5_needed (void) { //turn off 5v0 if host and can are off
	if (power_control_state & ((1<<ENABLE_CAN_RESET)|(1<<ENABLE_5V0B))) {
		PWR_turn_on((1<<ENABLE_5V0));
	} else {
		PWR_turn_off(1<<ENABLE_5V0);
	}
	PWR_push();
}

void PWR_can_stop (void) {
	PWR_turn_off((1<<ENABLE_CAN_RESET));
	PWR_is_5_needed();
	PWR_push();
};

void PWR_can_start (void) {
	PWR_turn_on((1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP));
	PWR_is_5_needed();
};

void PWR_gsm_stop(void) {
	PWR_turn_off((1<<ENABLE_SIM_RESET)|(1<<ENABLE_4V1));
	PWR_push();
}

void PWR_gsm_start(void) {
	PWR_turn_on((1<<ENABLE_SIM_RESET)|(1<<ENABLE_4V1));  //release the reset
	PWR_push();
	delay_ms(100);							//let power come up
	PWR_turn_on((1<<ENABLE_SIM_PWR_ON));
	PWR_push();
	delay_ms(150);							//let chip detect
	PWR_turn_off((1<<ENABLE_SIM_PWR_ON));
	PWR_push();					//clear the start bit
}

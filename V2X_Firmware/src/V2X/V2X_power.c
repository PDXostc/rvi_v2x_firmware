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
	#if V2X_REV >= REV_20

	/* Need to drive high 3v3_EN signal here, to ensure CPU operation.
	 * A later check will turn off 3v if we have chosen to enable 4v
	 */
	PWR_3_start();
	#endif
	PWR_clear();							// Clear shift register
	PWR_latch();							// Latch all internal registers to output
	power_control_state = POWER_CONTROL_DEFAULT_VALUE;
	#if V2X_REV >= REV_20
	/* Additional check, if 4v was enabled by default config, please disable 3v */
	PWR_3_is_needed();
	#endif
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
#if V2X_REV <= REV_12
/* With 2 shift registers, it is necessary to copy the power control values into
 * 2 packets, with MSB written first, so that the 2 byte sequence lands on the
 * register outputs in the proper order.
 */
	uint8_t data[2];
	data[1] = power_control_state & 0xff;
	data[0] = (power_control_state >> 8) & 0xff;
	spi_write_packet(SR_SPI, data, 2);
#elif V2X_REV >= REV_20
	/* We should be able to send the single byte to the single shift register,
	 * as long as the bit order has been respected.
	 */
	spi_write_packet(SR_SPI, &power_control_state, 1);
#endif
	PWR_latch();
}

void PWR_turn_on (SHIFT_REGISTER_TYPE pins_mask) {  //updates the power state variable but does not update shift register
	power_control_state |= pins_mask;
}

void PWR_turn_off(SHIFT_REGISTER_TYPE pins_mask) {  //updates the power state variable but does not update shift register
	power_control_state &= ~(pins_mask);
}

Bool PWR_query(SHIFT_REGISTER_TYPE mask) {
	if ((power_control_state & mask) != 0) {return true;}
	else {return false;}
}

#if V2X_REV <= REV_12
void PWR_hub_start(void) {
	PWR_turn_on((1<<ENABLE_HUB));
	PWR_push();
}

void PWR_hub_stop(void){
	PWR_turn_off((1<<ENABLE_HUB));
	PWR_push();
}
#endif

#if V2X_REV >= REV_20
/* 3 volt power pin manipulation
 * Use with caution! 3v should only be enabled when not using 4v
 * When transitioning to 3v power only, enable 3v immediately before disabling
 * 4v.
 */
void PWR_3_start(void) {
	gpio_set_pin_high(PWR_3V3_PIN);
}

/* 3 volt power pin manipulation
 * Use with caution! Please disable 3v immediately after enabling 4v.
 * Disabling 3v AND 4v will result in power loss to CPU. V2X system will
 * require manual restart.
 */

void PWR_3_stop(void) {
	gpio_set_pin_low(PWR_3V3_PIN);
}

void PWR_3_is_needed(void) {
	/* Check to see if 3v should be switched off due to 4v being enabled
	 * One might expect that we'd provide for turning 3v on here as well.
	 * Testing has shown however, that manipulation of the 3v is timing critical,
	 * and should take place exactly when we mean it to. This is intended only
	 * as a safety check to ensure the 4v and 3v don't fight.
	 * */
	if (power_control_state & (1<<ENABLE_4V1))
	{
		PWR_3_stop();
	}
}

void PWR_4_start(void) {
	PWR_3_stop();
	PWR_turn_on(1<<ENABLE_4V1);
	PWR_push();
}

void PWR_4_stop(void) {
	PWR_turn_off((1<<ENABLE_SIM_RESET)|(1<<ENABLE_4V1));
	PWR_push();
	PWR_3_start();
}

void PWR_shutdown(void) {
	PWR_turn_off((1<<ENABLE_4V1)|        \
				 (1<<ENABLE_5V0)|        \
				 (1<<ENABLE_5V0B)|       \
				 (1<<ENABLE_CAN_SLEEP)|  \
				 (1<<ENABLE_CAN_RESET)|  \
				 (1<<ENABLE_SIM_PWR_ON)| \
				 (1<<ENABLE_SIM_RESET)|  \
				 (1<<ENABLE_SIM_WAKE));
	PWR_push();
	PWR_3_stop();
}

#endif

void PWR_5_start(void) {
	PWR_4_start();
	PWR_turn_on(1<<ENABLE_5V0);
}

void PWR_5_stop (void) {
	PWR_turn_off((1<<ENABLE_5V0)|(1<<ENABLE_5V0B)|(1<<ENABLE_CAN_RESET));
	PWR_push();
}

void PWR_is_5_needed (void) { //turn off 5v0 if host and can are off
	if (power_control_state & ((1<<ENABLE_CAN_RESET)|(1<<ENABLE_5V0B))) {
		PWR_5_start();
	} else {
		PWR_5_stop();
	}
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
#if V2X_REV <= REV_12
	GSM_command_power_off();
	PWR_turn_on(1<<ENABLE_SIM_PWR_ON);
#elif V2X_REV >= REV_20
	// GSM_command_power_off();
	PWR_turn_on(1<<ENABLE_SIM_PWR_ON);
#endif
	PWR_push();
}

void PWR_gsm_start(void) {
#if V2X_REV <= REV_12
	//FIXME: Trying one extra measure to get the sim to respond using only the power pin...
	PWR_turn_off(1<<ENABLE_SIM_RESET);
	PWR_push();
	delay_ms(500);
	PWR_turn_on((1<<ENABLE_SIM_RESET)|(1<<ENABLE_4V1));  //release the reset
	PWR_push();
	if (sim_power_status())
	{
		PWR_turn_off((1<<ENABLE_SIM_PWR_ON));
		PWR_push();
		delay_ms(500);
	}
#elif V2X_REV >= REV_20
	PWR_turn_on(1<<ENABLE_SIM_RESET);
	PWR_4_start();
#endif
 	if (PWR_query(1<<ENABLE_SIM_PWR_ON))
	{
		PWR_turn_off((1<<ENABLE_SIM_PWR_ON));
		PWR_push();
		delay_ms(500);
	}

	PWR_turn_on((1<<ENABLE_SIM_PWR_ON));
	PWR_push();
	delay_ms(250);
	PWR_turn_off((1<<ENABLE_SIM_PWR_ON));
	PWR_push();					//clear the start bit
}

/* Force GSM reset. Please use with caution */
void PWR_gsm_reset(void) {
	PWR_turn_off(1<<ENABLE_SIM_RESET);
	PWR_push();
	delay_ms(50);
	PWR_turn_on(1<<ENABLE_SIM_RESET);
	PWR_push();
}

void PWR_mode_high(void) {
	PWR_4_start();
	PWR_5_start();
	usb_tx_string_P(PSTR("Power Full"));
	udd_attach();
	ACL_set_sample_on();
	GSM_modem_init();
	CAN_elm_init();
	PWR_host_start();
}

void PWR_mode_low(void) {
	usb_tx_string_P(PSTR("Power 3v Only"));
	// do 3v only
	PWR_host_stop();
	PWR_5_stop();
	ACL_set_sample_off();
	PWR_4_stop();
	// maybe we'd like to force the leds to update here, just in case...
	led_1_off();
	led_2_off();
}
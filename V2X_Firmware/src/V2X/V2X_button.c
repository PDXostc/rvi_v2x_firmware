/*
 * V2X_button.c
 *
 * Created: 2/12/2016 12:46:27 PM
 *  Author: Jesse Banks
 */

#include "V2X.h"

uint32_t pressed_at;
Bool button_pressed;
Bool button_check_flag = false;
int button_delta;

void button_init(void) {
	int x = 0;
	while (x < 100) {
		if (button_read()) {
			x = 0;
		} else {
			x++;
		}
		delay_ms(1);
	}
	button_pressed = false;
}

void button_service(void) {
	if (button_pressed != button_read()) { //has the button state changes since last time?
		switch (button_pressed) {
		case true: //button is released
//			if (button_pressed) { //if previously pressed figure out for how long
				button_delta = time_get() - pressed_at; //calc press duration
				menu_send_BTN();
				USB_tx_string_P(PSTR("RELEASE:"));		// report to CMD interface
				menu_print_int(button_delta);
				menu_send_n_st();
				button_pressed = false;		//note the button was released, kinda reverse logic 
				//button_check_flag = true;	//flag button for checking
				job_set_timeout(SYS_PWR, 1); // cause handle_button_check to run
//			}
			break;
		case false:  //button is pressed, 
//			if (!button_pressed) { //if first time into detect capture press timestamp
				button_delta = 0; // reset delta
				pressed_at = time_get(); //store press time
				button_pressed = true;  //hold button state, kinda reverse logic 
				menu_send_BTN();
				USB_tx_string_P(PSTR("PRESS")); //report press event to CMD
				menu_send_n_st();
//			}
			break;
		}

	}
}

// Bool check_button(void) {
// 	return button_check_flag;
// }

// Bool button_reset_check(void) {
// 	button_check_flag = false;
// }

int button_get_delta(void) {
	return button_delta;
 }

// void button_reset_delta(void){
// 	button_delta = 0;
// }

void handle_button_check(int sec) {
	if (sec >= 5)
	{
		// do hard power off
		USB_tx_string_P(PSTR("Power Off\r\n"));
		PWR_host_stop();
		PWR_can_stop();
		PWR_gsm_stop();
		ACL_set_sample_off();
		PWR_shutdown();
// 	} else if (sec >= 3)
// 	{
// 		PWR_mode_low();
	} else	{
		PWR_mode_high();
		if (CSC_get_state() == CSC_car_state_running) {
			job_set_timeout(SYS_CAR_STATE_CHECK, 60); //provide time to connect before CSC pushes to low power mode
		}
	}
	//button_reset_delta();
}

/*
 * V2X_button.c
 *
 * Created: 2/12/2016 12:46:27 PM
 *  Author: jbanks2
 */ 

#include "V2X.h"

uint32_t pressed_at;
Bool button_pressed;
int delta;

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
		case true: //button was released (previously pressed), figure out ho long
			delta = time_get() - pressed_at; //calc press duration
			usb_tx_string_P(PSTR("BUTTON.RELEASE:"));		// report to CMD interface
			menu_print_int(delta);		
			usb_tx_string_P(PSTR("\r>"));  //re-prompt for user input
			button_pressed = false;		//note the button was released
			break;
		case false:  //button was pressed, capture press time
			pressed_at = time_get(); //store press time
			button_pressed = true;  //hold button state
			usb_tx_string_P(PSTR("BUTTON.PRESS\r>")); //report press event to CMD
			break;
		}		
		
	}
}
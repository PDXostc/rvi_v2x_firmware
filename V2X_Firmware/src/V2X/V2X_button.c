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
	while (button_read()) {
		delay_ms(1);
	}
	button_pressed = false;  
}

void button_service(void) {
	if (button_pressed != button_read()) { //has the button state changes since last time?
		switch (button_pressed) {
		case true: //button was released (previously pressed), figure out ho long
//			delta = rtc_get_time() - pressed_at; //calc press durration
			usb_tx_string_P(PSTR("BUTTON.RELEASE:"));		// report to CMD interface
			menu_print_int(delta);		
			button_pressed = false;					//note the button was released
			break;
		case false:  //button was pressed, capture press time
//			pressed_at = rtc_get_time(); //store press time
			button_pressed = true;
			usb_tx_string_P(PSTR("BUTTON.PRESS"));
			break;
		}		
		
	}
}
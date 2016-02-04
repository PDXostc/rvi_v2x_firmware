/*
 * V2X_drivers.h
 *
 * Created: 11/24/2015 1:36:50 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_DRIVERS_H_
#define V2X_DRIVERS_H_

#include "V2X_board.h"

#define BUFFER_AVR_ROUTING 0
#define BUFFER_FTDI_ROUTING 1

void toggleChargePump(void);
inline static void chargePumpToggle(void) {gpio_toggle_pin(CHARGEPUMP_0_PIN); }
inline static void ChargePumpLow(void) {gpio_set_pin_low(CHARGEPUMP_0_PIN);}
inline static void ChargePumpHigh(void) {gpio_set_pin_high(CHARGEPUMP_0_PIN);}

inline static void led_0_on		(void) {gpio_set_pin_high(LED_0_PIN);	}
inline static void led_0_off	(void) {gpio_set_pin_low(LED_0_PIN);	}		  
inline static void led_1_on		(void) {gpio_set_pin_high(LED_1_PIN);	}
inline static void led_1_off	(void) {gpio_set_pin_low(LED_1_PIN);	}
inline static void led_2_on		(void) {gpio_set_pin_high(LED_2_PIN);	}
inline static void led_2_off	(void) {gpio_set_pin_low(LED_2_PIN);	}
	
/** SIMCARD detect function 
 * returns "true" if installed **/
inline static bool simcardStatus(void) {return 1-ioport_get_pin_level(SW1_PIN);}

/** Button detect function 
 * returns "true" if installed **/
inline static bool buttonStatus(void) {return ioport_get_pin_level(SW0_PIN);}

void canbusSerialRouting(uint8_t direction);

#endif /* V2X_DRIVERS_H_ */
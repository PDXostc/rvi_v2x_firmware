/*
 * V2X_button.h
 *
 * Created: 2/12/2016 12:46:38 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_BUTTON_H_
#define V2X_BUTTON_H_

/**
 * @def button_read
 * @brief detection of Button state 
 * returns "true" if button depressed
 **/
inline static bool button_read(void) {return ioport_get_pin_level(SW0_PIN);}

#endif /* V2X_BUTTON_H_ */
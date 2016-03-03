/*
 * V2X_button.h
 *
 * Created: 2/12/2016 12:46:38 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_BUTTON_H_
#define V2X_BUTTON_H_

/**
 * @def button_init
 * @brief waits to start the board until the button is released 
 * @brief sets "pressed" to false as init
 **/
void button_init(void);

/**
 * @def button_read
 * @brief detection of Button state 
 * returns "true" if button depressed
 **/
inline static bool button_read(void) {return ioport_get_pin_level(SW0_PIN);}

/**
 * @def button_service
 * @brief check Button state process changes reportr state to CMD interface
 * returns "true" if button depressed
 **/
void button_service(void);

#endif /* V2X_BUTTON_H_ */
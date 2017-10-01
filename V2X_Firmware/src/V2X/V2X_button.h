/*
 * V2X_button.h
 *
 * Created: 2/12/2016 12:46:38 PM
 *  Author: Jesse Banks
 */


#ifndef V2X_BUTTON_H_
#define V2X_BUTTON_H_

int button_delta;
Bool button_check_flag;

/**
 * @def button_init
 * @brief waits to start the board until the button is released
 * @brief sets "pressed" to false as init
 **/
void button_init(void);

/**
 * @def button_read
 * @brief detection of Button state
 * @retval "true" if button depressed
 **/
inline static bool button_read(void) {return ioport_get_pin_level(SW0_PIN);}

/**
 * @def button_service
 * @brief check Button state process changes reporter state to CMD interface
 * @retval "true" if button depressed
 **/
void button_service(void);

/**
 * @def check_button
 * @brief reports if a new button press has occured
 * @retval Bool, true if pressed
 **/
Bool check_button(void);

/**
 * @def button_reset_check
 * @brief clears the flag that reports a button press
 * @retval 
 **/
Bool button_reset_check(void);

/**
 * @def button_get_delta
 * @brief get the time delta of the last button press
 * @retval number of seconds
 **/
int button_get_delta(void);

/**
 * @def button_reset_delta
 * @brief clears the last delta result to 0
 **/
void button_reset_delta(void);

/**
 * @def handle_button_check
 * @brief run logic to interpret button press length
 * @param number of seconds the button was pressed
 **/
void handle_button_check(int sec);

#endif /* V2X_BUTTON_H_ */
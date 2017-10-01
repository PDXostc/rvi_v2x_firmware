/*
 * V2X_led.h
 *
 * Created: 2/12/2016 11:02:14 AM
 *  Author: Jesse Banks
 */


#ifndef V2X_LED_H_
#define V2X_LED_H_

/* Revision 2.0:
 * LED pins have been remapped from the ADC (analog-digital) channel ports to
 * OC (output compare). Currently, the pins are driven completely in software,
 * on update, which is undesireable and inefficient.
 * In the future, the LEDs should be driven by using PWM from Timer0, so that
 * updates are sent only when necessary, and the LEDs are driven by hardware
 * peripheral.
 */

/**
 * @def led_0_on
 * @brief Causes the pin driving the RED POWER LED to turn ON
 **/
inline static void led_red_on		(void) {gpio_set_pin_low(LED_0_PIN);	}

/**
 * @def led_0_off
 * @brief Causes the pin driving the RED POWER LED to turn OFF
 **/
inline static void led_red_off	(void) {gpio_set_pin_high(LED_0_PIN);	}

/**
 * @def led_1_on
 * @brief Causes the pin driving the GREEN GSM LED to turn ON
 **/
inline static void led_green_on		(void) {gpio_set_pin_low(LED_1_PIN);	}

/**
 * @def led_1_off
 * @brief Causes the pin driving the GREEN GSM LED to turn OFF
 **/
inline static void led_green_off	(void) {gpio_set_pin_high(LED_1_PIN);	}

/**
 * @def led_2_on
 * @brief Causes the pin driving the BLUE GPS LED to turn ON
 **/
inline static void led_blue_on		(void) {gpio_set_pin_low(LED_2_PIN);	}

/**
 * @def led_2_off
 * @brief Causes the pin driving the BLUE GPS LED to turn OFF
 **/
inline static void led_blue_off	(void) {gpio_set_pin_high(LED_2_PIN);	}

/**
 * @def led_update
 * @brief Updates the leds
 **/
void led_update(void);

/**
 * @def led_init
 * @brief starts led pwm routine
 **/
void led_init(void);

/**
 * @def flash_red
 * @brief makes the red led go bright
 **/
void flash_red (void);

/**
 * @def 
 * @brief 
 * @param 
 * @retval
 **/
#endif /* V2X_LED_H_ */
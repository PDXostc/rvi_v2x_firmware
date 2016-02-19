/*
 * V2X_led.h
 *
 * Created: 2/12/2016 11:02:14 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_LED_H_
#define V2X_LED_H_

/**
 * @def charge_pump_toggle
 * @brief Causes the pin driving the charge pump circuit to toggle state HIGH<>LOW
 **/
inline static void charge_pump_toggle(void) {gpio_toggle_pin(CHARGEPUMP_0_PIN); }

/**
 * @def led_0_on
 * @brief Causes the pin driving the RED POWER LED to turn ON
 **/
inline static void led_0_on		(void) {gpio_set_pin_high(LED_0_PIN);	}

/**
 * @def led_0_off
 * @brief Causes the pin driving the RED POWER LED to turn OFF
 **/
inline static void led_0_off	(void) {gpio_set_pin_low(LED_0_PIN);	}		  

/**
 * @def led_1_on
 * @brief Causes the pin driving the GREEN GSM LED to turn ON
 **/
inline static void led_1_on		(void) {gpio_set_pin_high(LED_1_PIN);	}

/**
 * @def led_1_off
 * @brief Causes the pin driving the GREEN GSM LED to turn OFF
 **/
inline static void led_1_off	(void) {gpio_set_pin_low(LED_1_PIN);	}

/**
 * @def led_2_on
 * @brief Causes the pin driving the BLUE GPS LED to turn ON
 **/
inline static void led_2_on		(void) {gpio_set_pin_high(LED_2_PIN);	}

/**
 * @def led_2_off
 * @brief Causes the pin driving the BLUE GPS LED to turn OFF
 **/
inline static void led_2_off	(void) {gpio_set_pin_low(LED_2_PIN);	}

/**
 * @def led_update
 * @brief Updates the leds
 **/
void led_update(void);

#endif /* V2X_LED_H_ */
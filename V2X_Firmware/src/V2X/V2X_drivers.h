/**
 * /file V2X_drivers.h
 *
 * /brief Hardware driver functions for operating the V2X board
 *
 * Author: Jesse Banks (jbanks2)
 **/

#ifndef V2X_DRIVERS_H_
#define V2X_DRIVERS_H_

#include "V2X_board.h"

/**
 * @def buffer_routing
 * @brief The two states the serial buffer can have
 **/
enum buffer_routing{
	AVR_ROUTING,
	FTDI_ROUTING
	};

/** 
 * @def SPI struct for shift register SPI device
 **/
struct spi_device sr_device_conf;
	
/** 
 * @def SPI struct for accelerometer SPI device
 **/
struct spi_device acl_device_conf;
	
/**
 * @def spi_start
 * @brief configures and enables the SPI peripheral
 **/
void spi_start(void);

/**
 * @def spi_write_read_packet
 * @brief sends a multi byte packet, returns the received bytes in the same data array
 * @param spi Base address of the SPI instance.
 * @param data The data byte array to be sent
 * @length size of message in bytes
 **/
void spi_write_read_packet (SPI_t* spi, uint8_t* data, uint8_t length);

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
 * @def simcard_status
 * @brief detection of SIMCARD by socket switch 
 * returns "true" if SIMCARD installed 
 **/
 inline static bool simcard_status(void) {return 1-ioport_get_pin_level(SW1_PIN);}

/**
 * @def button_read
 * @brief detection of Button state 
 * returns "true" if button depressed
 **/
inline static bool button_read(void) {return ioport_get_pin_level(SW0_PIN);}

/**
 * @def canbus_serial_routing
 * @brief Routing of serial data.
 * @param source = FTDI_ROUTING, AVR_ROUTING
 */
void canbus_serial_routing(uint8_t direction);

/**
 * @def shift_register_state
 * @brief variable that holds the current power state.
 */
static volatile uint16_t shift_register_state;

/**
 * @def STATE_DEFAULT_VALUE
 * @brief default power state. enables 3v3 only
 */
#define STATE_DEFAULT_VALUE (1<<ENABLE_3V3)|(0<<ENABLE_3V3B)|(0<<ENABLE_3V3C)|(0<<ENABLE_3V3D)|\
							(0<<ENABLE_4V1)|(0<<ENABLE_5V0)|(0<<ENABLE_5V0B)|(0<<ENABLE_SIM_WAKE)|\
							(0<<ENABLE_HUB)|(0<<ENABLE_CAN_SLEEP)|(0<<ENABLE_CAN_RESET)|(0<<ENABLE_SIM_PWR_ON)|\
							(0<<ENABLE_SIM_RESET)|(0<<ENABLE_SIM_RF_OFF)|(0<<ENABLE_SIM_VBUS)|(0<<ENABLE_FTDI_RESET)

/**
 * @def shift_register_init
 * @brief	Resets the shift register, then sets to default values
 *			this turns on power supplies to maintain the atmel and other circuits
 *			not very specific yet, all function just turn on.
 **/
void shift_register_init(void);

/**
 * @def shift_register_latch
 * @brief	causes the serial data just shifted into the shift register to latch to the outputs
 **/
void shift_register_latch(void);

/**
 * @def shift_register_clear
 * @brief Causes the register to latch all outputs to low
 **/
void shift_register_clear(void);

/**
 * @def state_to_shift_register
 * @brief	Sends the current power state to the shift register and latches to the outputs
 **/
void state_to_shift_register(void);

/**
 * @def turn_on
 * @brief Sets bits in the power state variable, in preparation for a state change
 * @param pins_mask bit wise mask of enable/power pins
 **/
void turn_on(uint16_t pins_mask);

/**
 * @def shift_register_clear
 * @brief Clears bits in the power state variable, in preparation for a state change
 * @param pins_mask bit wise mask of enable/power pins
 **/
void turn_off(uint16_t pins_mask);

uint8_t host_is_on (void);
#endif /* V2X_DRIVERS_H_ */
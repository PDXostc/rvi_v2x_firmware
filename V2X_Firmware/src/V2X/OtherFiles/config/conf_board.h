/**
 * \file conf_board.h
 *
 * \brief V2X power sequencer board definition V2X using XMEGA128A4U
 *
 * Created: 9/17/2017 11:01:25 PM
 * Author: Jesse Banks 
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* file load tracking */
#define V2X_CONF_BOARD_DEF

/* Board revision options */
#define REV_12 		(12)
#define REV_20		(20)

/* Board revision in use */
#define V2X_REV		(REV_20)

/** Name string macro */
/* REV20: board name change */
#define BOARD_NAME	"RVI V2X Version 2.1"
/** @} */

/* Defines for SIMCOM GSM chip on board. Compile time choice.
 */
#define SIMCOM_SIM5320A		(0)
#define SIMCOM_SIM7100A		(1)
//#define SIMCOM 				SIMCOM_SIM7100A
#define SIMCOM 				SIMCOM_SIM5320A

/* Configure SIMCOM to start the GPS automatically, without explicit instruction
 */
#define GPS_AUTO

/** \name 3v3 power pin definition
 * Activate to maintain power to CPU
 */
#define PWR_3V3_PIN				  IOPORT_CREATE_PIN(PORTA,4)
#define PWR_3V3_ACTIVE			  true
#define PWR_3V3_INACTIVE		  !PWR_3V3_ACTIVE

// power pin upgrade for 4v1 to 3v3 LDO power regulator
// waiting for a small PCB rev to be used
// Developed for a blue wire test performed by Jesse Banks
// On Board Rev 2.1, connect U20 Pin4 to U12 Pin3 
// Add LV Schottky diode from 4V1 to U12 pin1
#define PWR_4TO3_PIN			  IOPORT_CREATE_PIN(PORTB,0)
#define PWR_4TO3_ACTIVE			  true
#define PWR_3TO3_INACTIVE		  !PWR_3V3_ACTIVE

/** hard led on and off
* or smooth fading pwm */
#define LED_PWM
//#define LED_IO

#ifdef LED_PWM
	#define LED_PWM_FREQ 500 //Hz
	//Values are inverse logic
	//100:off		90:dim		0: bright
	#define LED_MAX_RED 95
	#define LED_FLASH_RED 10
	#define LED_MAX_GREEN 99
	#define LED_MAX_BLUE 90
	#define LED_DIM 100
	
#endif

/** \name LED0 definitions
	*  net: Red or "PWR" LED
	*  @{ */
#define LED0_PIN                  IOPORT_CREATE_PIN(PORTE,2)
#define LED0_ACTIVE               true
#define LED0_INACTIVE             !LED0_ACTIVE
#define LED_0_NAME                "LED0 (PWR, Red)"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
#define LED0_GPIO 				  LED0_PIN
/** @} */

/** \name LED1 definitions
	*  net: Blue or "M2M" LED
	*  @{ */
#define LED1_PIN                  IOPORT_CREATE_PIN(PORTE,1)
#define LED1_ACTIVE               true
#define LED1_INACTIVE             !LED0_ACTIVE
#define LED_1_NAME                "LED1 (M2M, Blue)"
#define LED_1_PIN                 LED1_PIN
#define LED_1_ACTIVE              LED1_ACTIVE
#define LED_1_INACTIVE            LED1_INACTIVE
#define LED1_GPIO 				  LED1_PIN
/** @} */

/** \name LED2 definitions
	*  net: Green or "GPS" LED
	*  @{ */
#define LED2_PIN                  IOPORT_CREATE_PIN(PORTE,3)
#define LED2_ACTIVE               true
#define LED2_INACTIVE             !LED0_ACTIVE
#define LED_2_NAME                "LED2 (GPS, Green)"
#define LED_2_PIN                 LED2_PIN
#define LED_2_ACTIVE              LED2_ACTIVE
#define LED_2_INACTIVE            LED2_INACTIVE
#define LED2_GPIO 				  LED2_PIN
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 3

/** \name SW0 definitions
 *  @{ */
#define SW0_PIN                   IOPORT_CREATE_PIN(PORTA,0)
#define SW0_ACTIVE                true
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_INT_VECT_0			  PORTA_INT0_vect
/** @} */

/**
 * \name Button #0 definitions
 *
 * User Button Input
 *
 *  @{ */
#define BUTTON_0_NAME             "Button"
#define BUTTON_0_PIN              SW0_PIN
#define BUTTON_0_ACTIVE           SW0_ACTIVE
#define BUTTON_0_INACTIVE         SW0_INACTIVE
/** @} */

/** \name SW1 definitions
 *  @{ */
#define SW1_PIN                   IOPORT_CREATE_PIN(PORTD,5)
#define SW1_ACTIVE                FALSE
#define SW1_INACTIVE              !SW1_ACTIVE
/** @} */

/**
 * \name Button #1 definitions
 *
 * SIMCARD insertion detection
 *
 *  @{ */
#define BUTTON_1_NAME             "SIMCARD Detect"
#define BUTTON_1_PIN              SW1_PIN
#define BUTTON_1_ACTIVE           SW1_ACTIVE
#define BUTTON_1_INACTIVE         SW1_INACTIVE
/** @} */

#define BUTTON_COUNT 2

/** \name CP0 definitions
 *  @{ */
#define CP0_PIN                   IOPORT_CREATE_PIN(PORTA,1)
#define CP0_ACTIVE                true
#define CP0_INACTIVE              !CP0_ACTIVE
/** @} */

/** \name Signal pin definitions
 *  @{
 */
#define EXT1_PIN_ACL_INT2                  IOPORT_CREATE_PIN(PORTA,1)
#define EXT1_PIN_ACL_INT1                  IOPORT_CREATE_PIN(PORTA,2)
#define EXT1_PIN_HUB_STATUS                IOPORT_CREATE_PIN(PORTA,3)
#define EXT1_PIN_HOST_SHORT_CIRCUIT        IOPORT_CREATE_PIN(PORTB,1)
#define EXT1_PIN_SR_CLEAR                  IOPORT_CREATE_PIN(PORTC,0)
#define EXT1_PIN_SR_LATCH                  IOPORT_CREATE_PIN(PORTC,1)
#define EXT1_PIN_CAN_TXD                   IOPORT_CREATE_PIN(PORTC,2) //TX SIGNAL (PORT C RXD0) FROM CAN MODULE
#define EXT1_PIN_CAN_RXD                   IOPORT_CREATE_PIN(PORTC,3) //RX SIGNAL (PORT C TXD0) TO CAN MODULE BUFFERED
#define EXT1_PIN_SPI_SS                    IOPORT_CREATE_PIN(PORTC,4)
#define EXT1_PIN_SPI_MOSI                  IOPORT_CREATE_PIN(PORTC,5)
#define EXT1_PIN_SPI_MISO                  IOPORT_CREATE_PIN(PORTC,6)
#define EXT1_PIN_SPI_SCK                   IOPORT_CREATE_PIN(PORTC,7)
#define EXT1_PIN_SIM_WAKE                  IOPORT_CREATE_PIN(PORTD,0)
#define EXT1_PIN_SIM_NETWORK               IOPORT_CREATE_PIN(PORTD,1)
#define EXT1_PIN_SIM_TXD                   IOPORT_CREATE_PIN(PORTE,2) //TX SIGNAL (PORT D RXD0) FROM SIM MODULE
#define EXT1_PIN_SIM_RXD                   IOPORT_CREATE_PIN(PORTE,3) //RX SIGNAL (PORT D TXD0) FROM SIM MODULE
#define EXT1_PIN_SIM_PWR                   IOPORT_CREATE_PIN(PORTD,4)
#define EXT1_PIN_SEQ_RXD                   IOPORT_CREATE_PIN(PORTD,2) //RX SIGNAL (PORT D RXD1) FROM FTDI MODULE FOR SEQ AT INTERFACE, ALSO USB DM
#define EXT1_PIN_SEQ_TXD                   IOPORT_CREATE_PIN(PORTD,3) //TX SIGNAL (PORT D TXD1) TO FTDI MODULE FOR SEQ AT INTERFACE, ALSO USB DP
/** @} */

/*! \name SPI Connections of external ADXL345 Accelerometer
 *
 * This is mainly used for proper board compilation
 *
 */

//! @{
#define ACL_SPI				  &SPIC
#define ACL_CS			      EXT1_PIN_SPI_SS
#define ACL_MASTER_SCK	      EXT1_PIN_SPI_SCK // SCK as output
#define ACL_MASTER_SS         EXT1_PIN_SPI_SS  // SS as output
#define ACL_MASTER_MOSI       EXT1_PIN_SPI_MOSI // MOSI as output
#define ACL_MASTER_MISO       EXT1_PIN_SPI_MISO // MISO as input
#define ACL_SPI_CLK_MASK      SYSCLK_PORT_C
//! @}

/*! \name Shift Register Connections
 *
 * This is used for power and reset sequencing
 *
 */
/* Revision 2.0 uses only a single shift register, accommodating an 8 bit field
 * for the following power signals. Note: order is relevant!
 */
 enum power_sequence_outputs {
	 ENABLE_4V1			= 0
	 , ENABLE_5V0
	 , ENABLE_5V0B
	 , ENABLE_CAN_SLEEP
	 , ENABLE_CAN_RESET
	 , ENABLE_SIM_PWR_ON
	 , ENABLE_SIM_RESET
	 , ENABLE_SIM_WAKE
 };

#define SR_SPI				&SPIC
//! @}


enum usb_port_usage{  //usually used in port variables for USB functions
	USB_CAN	= 0,
	USB_CMD,
	USB_ACL
	};
	
#ifdef __cplusplus
}
#endif

#endif // CONF_BOARD_H

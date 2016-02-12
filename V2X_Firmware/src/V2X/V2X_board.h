/**
 * \file V2X_board.h
 *
 * \brief V2X power sequencer board definition XMEGA128A4U 
 *
 * Author: Jesse Banks (jbanks2)
 */

#ifndef V2X_BOARD_H
#define V2X_BOARD_H

#include <../asf.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup xmega_a1u_xplained_pro_group Features
 *
 * Symbols that describe features and capabilities of the board.
 *
 * @{
 */

/** Name string macro */
#define BOARD_NAME                "RVI_V2X_Version_1.2"
/** @} */

/** \name LED0 definitions
 *  net: SEQ_LED3 or "PWR" LED
 *  @{ */
#define LED0_PIN                  IOPORT_CREATE_PIN(PORTB, 0)
#define LED0_ACTIVE               true
#define LED0_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name LED1 definitions
 *  net: SEQ_LED1 or "M2M" LED
 *  @{ */
#define LED1_PIN                  IOPORT_CREATE_PIN(PORTA, 6)
#define LED1_ACTIVE               true
#define LED1_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name LED2 definitions
 *  net: SEQ_LED3 or "GPS" LED
 *  @{ */
#define LED2_PIN                  IOPORT_CREATE_PIN(PORTA, 7)
#define LED2_ACTIVE               true
#define LED2_INACTIVE             !LED0_ACTIVE
/** @} */


/**
 * \name LED #1 definitions
 *
 * net: SEQ_LED1 or "M2M" LED
 *
 *  @{ */
#define LED_1_NAME                "LED1 (M2M, Green)"
#define LED_1_PIN                 LED1_PIN
#define LED_1_ACTIVE              LED1_ACTIVE
#define LED_1_INACTIVE            LED1_INACTIVE
#define LED1_GPIO 				  LED1_PIN
/** @} */

/**
 * \name LED #2 definitions
 *
 * net: SEQ_LED2 or "GPS" LED
 *
 *  @{ */
#define LED_2_NAME                "LED2 (GPS, Blue)"
#define LED_2_PIN                 LED2_PIN
#define LED_2_ACTIVE              LED2_ACTIVE
#define LED_2_INACTIVE            LED2_INACTIVE
#define LED2_GPIO 				  LED2_PIN
/** @} */

/**
 * \name LED #0 definitions
 *
 * net: SEQ_LED3 or "PWR" LED
 *
 *  @{ */
#define LED_0_NAME                "LED0 (PWR, Red)"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
#define LED0_GPIO 				  LED0_PIN
/** @} */

/** Number of on-board LEDs */
#define LED_COUNT                 3

/** \name SW0 definitions
 *  @{ */
#define SW0_PIN                   IOPORT_CREATE_PIN(PORTA, 0)
#define SW0_ACTIVE                true
#define SW0_INACTIVE              !SW0_ACTIVE
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
#define SW1_PIN                   IOPORT_CREATE_PIN(PORTD, 5)
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

/**
 * \name Serial Buffer #0 definitions
 *
 * Makes Atmel active communicator with ELM/STN device
 *
 *  @{ */
#define BUF_0_NAME             "Buffer SEQ Active"
#define BUF_0_PIN              BUF0_PIN
#define BUF_0_ACTIVE           BUF0_ACTIVE
#define BUF_0_INACTIVE         BUF0_INACTIVE
/** @} */

/** \name BUF0 definitions
 *  @{ */
#define BUF0_PIN                   IOPORT_CREATE_PIN(PORTB, 2)
#define BUF0_ACTIVE                true
#define BUF0_INACTIVE              !BUF0_ACTIVE
/** @} */

/**
 * \name Serial Buffer #1 definitions
 *
 * Makes FTDI active communicator with ELM/STN device
 *
 *  @{ */
#define BUF_1_NAME             "Buffer FTDI Active"
#define BUF_1_PIN              BUF1_PIN
#define BUF_1_ACTIVE           BUF1_ACTIVE
#define BUF_1_INACTIVE         BUF1_INACTIVE
/** @} */

/** \name BUF1 definitions
 *  @{ */
#define BUF1_PIN                   IOPORT_CREATE_PIN(PORTB, 3)
#define BUF1_ACTIVE                true
#define BUF1_INACTIVE              !BUF1_ACTIVE
/** @} */

/** Number of on-board buttons */
#define BUFFER_COUNT 2

/** \name CP0 definitions
 *  @{ */
#define CP0_PIN                   IOPORT_CREATE_PIN(PORTA, 1)
#define CP0_ACTIVE                true
#define CP0_INACTIVE              !CP0_ACTIVE
/** @} */

/**
 * \name Charge Pump #0 definitions
 *
 * LED power supply charge pump pin
 * must be toggled >10khz to activate blue LED
 *
 *  @{ */
#define CHARGEPUMP_0_NAME             "Charge Pump"
#define CHARGEPUMP_0_PIN              CP0_PIN
#define CHARGEPUMP_0_ACTIVE           CP0_ACTIVE
#define CHARGEPUMP_0_INACTIVE         CP0_INACTIVE
/** @} */

/** Number of on-board buttons */
#define CHARGEPUMP_COUNT 1

/** \name Signal pin definitions
 *  @{
 */
#define EXT1_PIN_ACL_INT2                  IOPORT_CREATE_PIN(PORTA,2)
#define EXT1_PIN_ACL_INT1                  IOPORT_CREATE_PIN(PORTA,3)
#define EXT1_PIN_HUB_STATUS                IOPORT_CREATE_PIN(PORTA,4)
#define EXT1_PIN_HUB_SUSPEND               IOPORT_CREATE_PIN(PORTE,5)
#define EXT1_PIN_HOST_SHORT_CIRCUIT        IOPORT_CREATE_PIN(PORTB,1)
#define EXT1_PIN_SR_CLEAR                  IOPORT_CREATE_PIN(PORTC,0)
#define EXT1_PIN_SR_LATCH                  IOPORT_CREATE_PIN(PORTC,1)
#define EXT1_PIN_CAN_TXD                   IOPORT_CREATE_PIN(PORTC,2) //TX SIGNAL (PORT C RXD0) FROM CAN MODULE
#define EXT1_PIN_CAN_RXD                   IOPORT_CREATE_PIN(PORTC,3) //RX SIGNAL (PORT C TXD0) TO CAN MODULE BUFFERED
#define EXT1_PIN_SPI_SS                    IOPORT_CREATE_PIN(PORTC,4)
#define EXT1_PIN_SPI_MOSI                  IOPORT_CREATE_PIN(PORTC,5)
#define EXT1_PIN_SPI_MISO                  IOPORT_CREATE_PIN(PORTC,6)
#define EXT1_PIN_SPI_SCK                   IOPORT_CREATE_PIN(PORTC,7)
#define EXT1_PIN_HUB_SDA                   IOPORT_CREATE_PIN(PORTE,0)
#define EXT1_PIN_HUB_SCL                   IOPORT_CREATE_PIN(PORTE,1)
#define EXT1_PIN_ACL_RXD                   IOPORT_CREATE_PIN(PORTE,2) //RX SIGNAL (PORT E RXD0) FROM SEQ TO FTDI FOR ACCEL AT INTERFACE
#define EXT1_PIN_ACL_TXD                   IOPORT_CREATE_PIN(PORTE,3) //TX SIGNAL (PORT E TXD0) FROM SEQ TO FTDI FOR ACCEL AT INTERFACE
#define EXT1_PIN_SIM_WAKE                  IOPORT_CREATE_PIN(PORTD,0)
#define EXT1_PIN_SIM_NETWORK               IOPORT_CREATE_PIN(PORTD,1)
#define EXT1_PIN_SIM_TXD                   IOPORT_CREATE_PIN(PORTE,2) //TX SIGNAL (PORT D RXD0) FROM SIM MODULE
#define EXT1_PIN_SIM_RXD                   IOPORT_CREATE_PIN(PORTE,3) //RX SIGNAL (PORT D TXD0) FROM SIM MODULE
#define EXT1_PIN_SIM_PWR                   IOPORT_CREATE_PIN(PORTD,4)
#define EXT1_PIN_SEQ_RXD                   IOPORT_CREATE_PIN(PORTD,6) //RX SIGNAL (PORT D RXD1) FROM FTDI MODULE FOR SEQ AT INTERFACE, ALSO USB DM
#define EXT1_PIN_SEQ_TXD                   IOPORT_CREATE_PIN(PORTD,7) //TX SIGNAL (PORT D TXD1) TO FTDI MODULE FOR SEQ AT INTERFACE, ALSO USB DP
/** @} */


/** \name TWI definitions
 *  @{
 */
#define EXT1_TWI_MODULE             TWIA
#define TWI_MASTER					TWIA
#define TWI_MASTER_PORT				PORTA
#define TWI_SPEED					50000
#define TWI_MASTER_ADDR				0x10
#define TWI_DATA_LENGTH				8
/** @} */

/** \name Extension header #1 USART definitions
*  @{
*/
#define EXT1_CAN_USART              &USARTC0
#define EXT1_SIM_USART              &USARTD0
#define EXT1_SEQ_USART              &USARTD1
#define EXT1_ACL_USART              &USARTE0
/** @} */

 /** \name Extension header #1 SPI definitions
 *  @{
 */
#define EXT1_SPI_MODULE              &SPIC
 	
/** @} */
/** \name Extension header #2 SPI definitions
 *  @{
 */
#define EXT1_SPI_MODULE              &SPIC
 	
/** @} */

/*! \name SPI Connections of external ADXL345 Accelerometer
 *
 * This is mainly used for proper board compilation
 *
 */

//! @{
#define sck_pin				  EXT1_PIN_SPI_SCK
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

#define ENABLE_3V3			0
#define ENABLE_3V3B			1
#define ENABLE_3V3C			2
#define ENABLE_3V3D			3
#define ENABLE_4V1			4
#define ENABLE_5V0			5
#define ENABLE_5V0B			6
#define ENABLE_SIM_WAKE		7
#define ENABLE_HUB			8
#define ENABLE_CAN_SLEEP	9
#define ENABLE_CAN_RESET	10
#define ENABLE_SIM_PWR_ON	11
#define ENABLE_SIM_RESET	12
#define ENABLE_SIM_RF_OFF	13
#define ENABLE_SIM_VBUS		14
#define ENABLE_FTDI_RESET	15

#define SR_SPI				&SPIC
//! @}

#ifdef __cplusplus
}
#endif

#endif // V2X_BOARD_H

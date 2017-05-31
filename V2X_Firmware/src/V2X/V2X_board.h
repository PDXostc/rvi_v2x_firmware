/**
 * \file V2X_board.h
 *
 * \brief V2X power sequencer board definition XMEGA128A4U
 *
 * Author: Jesse Banks (jbanks2)
 */

#ifndef V2X_BOARD_H
#define V2X_BOARD_H

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

/* Board revision options */
#define REV_12 		(12)
#define REV_20		(20)

/* Board revision in use */
#define V2X_REV		(REV_20)

/** Name string macro */
/* REV20: board name change */
#define BOARD_NAME                "RVI_V2X_Version_2.0"
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
#if V2X_REV >= REV_20
#define PWR_3V3_PIN				  IOPORT_CREATE_PIN(PORTA,4)
#define PWR_3V3_ACTIVE			  true
#define PWR_3V3_INACTIVE		  !PWR_3V3_ACTIVE
#endif

/** \name LED0 definitions
 *  net: SEQ_LED3 or "PWR" LED
 *  @{ */
#define LED0_PIN                  IOPORT_CREATE_PIN(PORTE,2)
#define LED0_ACTIVE               true
#define LED0_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name LED1 definitions
 *  net: SEQ_LED1 or "M2M" LED
 *  @{ */
#define LED1_PIN                  IOPORT_CREATE_PIN(PORTE,1)
#define LED1_ACTIVE               true
#define LED1_INACTIVE             !LED0_ACTIVE
/** @} */

/** \name LED2 definitions
 *  net: SEQ_LED2 or "GPS" LED
 *  @{ */
#define LED2_PIN                  IOPORT_CREATE_PIN(PORTE,3)
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
#define EXT1_PIN_SEQ_RXD                   IOPORT_CREATE_PIN(PORTD,6) //RX SIGNAL (PORT D RXD1) FROM FTDI MODULE FOR SEQ AT INTERFACE, ALSO USB DM
#define EXT1_PIN_SEQ_TXD                   IOPORT_CREATE_PIN(PORTD,7) //TX SIGNAL (PORT D TXD1) TO FTDI MODULE FOR SEQ AT INTERFACE, ALSO USB DP
/* FIXME: Add definition for HUB_PWR_PRT1 pin, on PORTA,3 ?? */
/** @} */

/* FIXME: Is the following TWI definition incorrect, or irrelevant?
 * Incorrect because (there may not actually be a TWI device on PORTA)?
 * Irrelevant because we do not use TWI device>?
 */

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

#define  USART					USARTC0
#define  USART_RX_Vect			USARTC0_RXC_vect
#define  USART_DRE_Vect			USARTC0_DRE_vect
#define  USART_SYSCLK			SYSCLK_USART0
#define  USART_PORT				PORTC
#define  USART_PORT_PIN_TX		(1<<3)
#define  USART_PORT_PIN_RX		(1<<2)
#define  USART_PORT_SYSCLK		SYSCLK_PORT_C

#define  USART_BAUDRATE			9600
#define  USART_CHAR_LENGTH		USART_CHSIZE_8BIT_gc
#define  USART_PARITY			USART_PMODE_DISABLED_gc
#define  USART_STOP_BIT			false

#define  USART_SIM              USARTD0
#define  USART_SIM_RX_Vect      USARTD0_RXC_vect
#define  USART_SIM_DRE_Vect     USARTD0_DRE_vect
#define  USART_SIM_SYSCLK       SYSCLK_USART0
#define  USART_SIM_PORT         PORTD
#define  USART_SIM_PORT_PIN_TX  (1<<3)
#define  USART_SIM_PORT_PIN_RX  (1<<2)
#define  USART_SIM_PORT_SYSCLK  SYSCLK_PORT_D

#define  USART_SIM_BAUDRATE		115200
#define  USART_SIM_CHAR_LENGTH  USART_CHSIZE_8BIT_gc
#define  USART_SIM_PARITY       USART_PMODE_DISABLED_gc
#define  USART_SIM_STOP_BIT     false

enum usb_port_usage{  //usually used in port variables for USB functions
	USB_CAN	= 0,
	USB_CMD,
	USB_ACL
	};

#ifdef __cplusplus
}
#endif

#endif // V2X_BOARD_H

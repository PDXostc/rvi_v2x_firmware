/**
 * \file conf_usart_serial.h
 *
 *  Author: Jesse Banks
 *
 * \brief USART Serial configuration
 */

#ifndef CONF_USART_SERIAL_H_INCLUDED
#define CONF_USART_SERIAL_H_INCLUDED

#define  CAN_UART				&USARTC0
#define  CAN_RX_Vect			USARTC0_RXC_vect
#define  CAN_TX_Vect			USARTC0_TXC_vect
#define  CAN_SYSCLK				SYSCLK_USART0
#define  CAN_PORT				PORTC
#define  CAN_PORT_PIN_TX		(1<<3)
#define  CAN_PORT_PIN_RX		(1<<2)
#define  CAN_PORT_SYSCLK		SYSCLK_PORT_C
#define  CAN_BAUDRATE			9600
#define  CAN_CHAR_LENGTH		USART_CHSIZE_8BIT_gc
#define  CAN_PARITY				USART_PMODE_DISABLED_gc
#define  CAN_STOP_BIT			false

#define  SIM_UART				&USARTD0
#define  SIM_RX_Vect			USARTD0_RXC_vect
#define  SIM_TX_Vect			USARTD0_TXC_vect
#define  SIM_SYSCLK				SYSCLK_USART0
#define  SIM_PORT				PORTD
#define  SIM_PORT_PIN_TX		(1<<3)
#define  SIM_PORT_PIN_RX		(1<<2)
#define  SIM_PORT_SYSCLK		SYSCLK_PORT_D
#define  SIM_BAUDRATE			115200
#define  SIM_CHAR_LENGTH		USART_CHSIZE_8BIT_gc
#define  SIM_PARITY				USART_PMODE_DISABLED_gc
#define  SIM_STOP_BIT			false

#define V2X_UART_DEF

#endif /* CONF_USART_SERIAL_H_INCLUDED */

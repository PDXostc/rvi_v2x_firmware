/*
 * V2X_uart.h
 *
 * Created: 2/12/2016 10:34:20 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_UART_H_
#define V2X_UART_H_

/**
 * @def buffer_routing
 * @brief The two states the serial buffer can have
 **/
enum buffer_routing{
	AVR_ROUTING,
	FTDI_ROUTING
	};

/**
 * @def canbus_serial_routing
 * @brief Routing of serial data.
 * @param source = FTDI_ROUTING, AVR_ROUTING
 */
void canbus_serial_routing(uint8_t direction);


#endif /* V2X_UART_H_ */
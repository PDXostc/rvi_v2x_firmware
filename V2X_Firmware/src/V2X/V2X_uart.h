/*
 * V2X_uart.h
 *
 * Created: 2/12/2016 10:34:20 AM
 *  Author: Jesse Banks
 */


#ifndef V2X_UART_H_
#define V2X_UART_H_

/**
 * @def buffer_routing
 * @brief The two states the serial buffer can have
 **/
enum buffer_routing{
	AVR_ROUTING = 0
	};

/**
 * @def canbus_serial_routing
 * @brief Routing of serial data.
 * @param direction = FTDI_ROUTING, AVR_ROUTING
 */
void canbus_serial_routing(uint8_t direction);

/*! \brief Called by CDC interface
 * Callback running when CDC device have received data
 */
void uart_rx_notify(uint8_t port);

/*! \brief Configures communication line
 *
 * \param port     port definition
 * \param cfg      line configuration
 */
void uart_config(uint8_t port, usb_cdc_line_coding_t * cfg);

/*! \brief Opens communication line
 * \param port     port definition
 */
void uart_open(uint8_t port);

/*! \brief Closes communication line
 * \param port     port definition
 */
void uart_close(uint8_t port);


#endif /* V2X_UART_H_ */
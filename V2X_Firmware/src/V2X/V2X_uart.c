/*
 * V2X_uart.c
 *
 * Created: 2/12/2016 10:34:41 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

char buffer[30] = "XYZT: ";  //create starting string

void canbus_serial_routing(uint8_t source)
{
	gpio_set_pin_low(BUF0_PIN);
	gpio_set_pin_low(BUF1_PIN);
	if		(source == FTDI_ROUTING)	{gpio_set_pin_high(BUF1_PIN);}
	else  /*(source == AVR_ROUTING)*/	{gpio_set_pin_high(BUF0_PIN);}
}

void uart_config(uint8_t port, usb_cdc_line_coding_t * cfg)
{
	uint8_t reg_ctrlc;
	uint16_t bsel;

	reg_ctrlc = USART_CMODE_ASYNCHRONOUS_gc;

	switch (cfg->bCharFormat) {
		case CDC_STOP_BITS_2:
		reg_ctrlc |= USART_SBMODE_bm;
		break;
		case CDC_STOP_BITS_1:
		default:
		// Default stop bit = 1 stop bit
		break;
	}

	switch (cfg->bParityType) {
		case CDC_PAR_EVEN:
		reg_ctrlc |= USART_PMODE_EVEN_gc;
		break;
		case CDC_PAR_ODD:
		reg_ctrlc |= USART_PMODE_ODD_gc;
		break;
		default:
		case CDC_PAR_NONE:
		reg_ctrlc |= USART_PMODE_DISABLED_gc;
		break;
	}

	switch (cfg->bDataBits) {
		case 5:
		reg_ctrlc |= USART_CHSIZE_5BIT_gc;
		break;
		case 6:
		reg_ctrlc |= USART_CHSIZE_6BIT_gc;
		break;
		case 7:
		reg_ctrlc |= USART_CHSIZE_7BIT_gc;
		break;
		default:
		case 8:
		reg_ctrlc |= USART_CHSIZE_8BIT_gc;
		break;
		case 9:
		reg_ctrlc |= USART_CHSIZE_9BIT_gc;
		break;
	}

	sysclk_enable_module(USART_PORT_SYSCLK, USART_SYSCLK);
	// Set configuration
	USART.CTRLC = reg_ctrlc;
	// Update baudrate
	bsel = (uint16_t) (((((((uint32_t) sysclk_get_cpu_hz()) << 1) / ((uint32_t)
	le32_to_cpu(cfg->dwDTERate) * 8)) + 1) >> 1) - 1);
	USART.BAUDCTRLA = bsel & 0xFF;
	USART.BAUDCTRLB = bsel >> 8;
}

void uart_open(uint8_t port)
{
	sysclk_enable_module(USART_PORT_SYSCLK, USART_SYSCLK);
	// Open UART communication
	USART_PORT.DIRSET = USART_PORT_PIN_TX; // TX as output.
	USART_PORT.DIRCLR = USART_PORT_PIN_RX; // RX as input.

	// Enable both RX and TX
	USART.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;
	// Enable interrupt with priority higher than USB
	USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t)
	USART_DREINTLVL_OFF_gc;
}

void uart_close(uint8_t port)
{
	sysclk_disable_module(USART_PORT_SYSCLK, USART_SYSCLK);
	// Disable interrupts
	USART.CTRLA = 0;
	// Close RS232 communication
	USART.CTRLB = 0;
}

void uart_rx_notify(uint8_t port) //message received over USB
{
	uint8_t data;
	if (port == USB_CAN) {
		// If UART is open
		if (USART.CTRLA!=0) {
			// Enable UART TX interrupt to send values
			USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc | (register8_t)
			USART_DREINTLVL_HI_gc;
			}
		}else if (port == USB_CMD) {
			if (udi_cdc_multi_is_rx_ready(port)) {  //is there data
 				data = udi_cdc_multi_getc(port);	//get 1 char of data
				if (data == '?'						//allow '?'
				||  data == 0x7f					//backspace
				||  data == '\r'					//return
				|| (data >= '0' && data <= '9') 	//numbers
				|| (data >= 'A' && data <= 'Z') 	//capitals
				|| (data >= 'a' && data <= 'z')) {	//lower case
					if (!udi_cdc_multi_is_tx_ready(port)) {		//is TX ready
 						udi_cdc_multi_signal_overrun(port);		//no
 					} else {udi_cdc_multi_putc(port, data);}	//push char to loop back
					if (data == '\r') { //if carage return, run the menu
						menu_main();
						return;
					} else { //was a standard charecter that should be stored in the buffer
						menu_add_to_command(data);
					}
				} else { //there was a special character
					//run through the buffer until it is empty
					while (udi_cdc_multi_is_rx_ready(port)) {  
						data = udi_cdc_multi_getc(port);
					}
				}
			}
		}else if (port == USB_ACL) { //loop back
		while (udi_cdc_multi_is_rx_ready(port)) {  //is there data
			int data = udi_cdc_multi_getc(port);	//get all the data
			if (!udi_cdc_multi_is_tx_ready(port)) {		//is TX ready
				udi_cdc_multi_signal_overrun(port);		//no
			}else{udi_cdc_multi_putc(port, data);}
		}
	}
}

void report_accel_data(void) {
	uint8_t data[6];
	ACL_sample(data); //collect sample data
	ACL_data_to_string(data, buffer);
	usb_cdc_send_string(USB_ACL, buffer);
}

ISR(USART_RX_Vect)
{
	uint8_t value;
	if (0 != (USART.STATUS & (USART_FERR_bm | USART_BUFOVF_bm))) {
		udi_cdc_multi_signal_framing_error(USB_CAN);
	}
	// Transfer UART RX fifo to CDC TX
	value = USART.DATA;
	if (!udi_cdc_multi_is_tx_ready(USB_CAN)) {
		// Fifo full
		udi_cdc_multi_signal_overrun(USB_CAN);
		//ui_com_overflow();
		}else{
		udi_cdc_multi_putc(USB_CAN, value);
	}
	//ui_com_tx_stop();
}

ISR(USART_DRE_Vect)
{
	// Data send
	if (udi_cdc_is_rx_ready()) {
		// Transmit next data
		//ui_com_rx_start();
		USART.DATA = udi_cdc_getc();
		} else {
		// Fifo empty then Stop UART transmission
		USART.CTRLA = (register8_t) USART_RXCINTLVL_HI_gc |
		(register8_t) USART_DREINTLVL_OFF_gc;
		//ui_com_rx_stop();
	}
}

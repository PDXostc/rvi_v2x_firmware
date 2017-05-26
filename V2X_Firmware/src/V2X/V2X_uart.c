/*
 * V2X_uart.c
 *
 * Created: 2/12/2016 10:34:41 AM
 *  Author: jbanks2
 */

#include "V2X.h"

#if V2X_REV <= REV_12
/* DELETEME: Likely get rid of this entirely, like the aforementioned buffer cases */
void canbus_serial_routing(uint8_t source)
{
	gpio_set_pin_low(BUF0_PIN);

	/* use FTDI by default */
	gpio_set_pin_low(BUF1_PIN);
	if		(source == FTDI_ROUTING)	{gpio_set_pin_high(BUF1_PIN);}
	else  /*(source == AVR_ROUTING)*/	{gpio_set_pin_high(BUF0_PIN);}
	/* use Atmel */
	gpio_set_pin_high(BUF0_PIN);
}
#endif

void uart_config(uint8_t port, usb_cdc_line_coding_t * cfg)
{
	if (port != USB_CAN) {return;}

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
	bsel = (uint16_t) (((((((uint32_t) sysclk_get_cpu_hz()) << 1) / ((uint32_t) le32_to_cpu(cfg->dwDTERate) * 8)) + 1) >> 1) - 1);
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
	CAN_clear_tx_int();
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
		while (udi_cdc_multi_is_rx_ready(port)) {  //is there data
 			data = udi_cdc_multi_getc(port);	//get 1 char of data
// 			if (data == '?'						//allow '?'
// 			||  data == '-'						//minus sign
// 			||  data == '+'						//plus
// 			||  data == '='						//plus
// 			||  data == '\"'						//plus
// 			||  data == 0x7f					//backspace
// 			||  data == '\r'					//return
// 			|| (data >= '0' && data <= '9') 	//numbers
// 			|| (data >= 'A' && data <= 'Z') 	//capitals
// 			|| (data >= 'a' && data <= 'z')) {	//lower case
//
	 		if (data >= 0x20 && data <= 0x7F || data == '\r' || data == '\n' || data == 8) {
				if (!udi_cdc_multi_is_tx_ready(port)) {		//is TX ready
 					udi_cdc_multi_signal_overrun(port);		//no
 				} else {udi_cdc_multi_putc(port, data);}	//push char to loop back
				if (data == '\r' || data == '\n') { //if carriage return, run the menu
					menu_main();
					return;
				} else { //was a standard character that should be stored in the buffer
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
			}else{
				if (data == 'u' || data == 'U') {
					reset_trigger_USB();
				} else if (data == 'r' || data == 'R') {
					reset_trigger_SYSTEM();
				}
				udi_cdc_multi_putc(port, data);
			}
		}
	}
}

ISR(USART_RX_Vect)
{
	uint8_t value = USART.DATA;
	if (CAN_is_controlled() || CAN_is_snooping() || usb_cdc_is_active(USB_CAN) == false) {
		CAN_new_data(value);
	}

	if (usb_cdc_is_active(USB_CAN) == true){
		//host is on, send over USB
		if (0 != (USART.STATUS & (USART_FERR_bm | USART_BUFOVF_bm))) {
			udi_cdc_multi_signal_framing_error(USB_CAN);
		}
		// Transfer UART RX fifo to CDC TX
		if (!udi_cdc_multi_is_tx_ready(USB_CAN)) {
			// Fifo full
			udi_cdc_multi_signal_overrun(USB_CAN);
		}else{
			udi_cdc_multi_putc(USB_CAN, value);
		}
	}
}

ISR(USART_DRE_Vect)
{
	if (CAN_is_controlled() || CAN_is_snooping() || !usb_cdc_is_active(USB_CAN) == true) { //can controller needs to send data
		CAN_send_data();
	}

	if (usb_cdc_is_active(USB_CAN) == true){  //usb is in control
		// Data from USB
		if (udi_cdc_is_rx_ready()) {
			// Transmit next data
			USART.DATA = udi_cdc_getc();
		} else {
			// Fifo empty then Stop UART transmission
			CAN_clear_tx_int();
		}
	}
}

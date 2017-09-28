/*
 * V2X_usb.c
 *
 * Created: 9/17/2017 3:39:25 PM
 *  Author: Jesse Banks
 */ 

#ifndef V2X_H_
  #include "V2X.h"
#endif

static volatile bool usb_cdc_enabled_bool[3] = {false, false, false};

 void USB_callback_vbus_action(bool b_vbus_high)
 {
 	//the host has come or gone as the vbus pin has changed state
}
 
void USB_callback_sof_action(void)  
{
	//called every ms while usb is active. could reset a WDT 
}

void USB_callback_suspend_action(void)
{
	//operation to perform when USB unplugged
}

void USB_callback_resume_action(void)
{
	//operation to perform when USB starts
}

void USB_callback_config(uint8_t port, usb_cdc_line_coding_t * cfg)
{
	uint8_t reg_ctrlc = 0;
	uint16_t bsel = 0;

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

	sysclk_enable_module(CAN_PORT_SYSCLK, CAN_SYSCLK);
	// Set configuration
	(CAN_UART)->CTRLC = reg_ctrlc;
	// Update baudrate
	bsel = (uint16_t) (((((((uint32_t) sysclk_get_cpu_hz()) << 1) / ((uint32_t) le32_to_cpu(cfg->dwDTERate) * 8)) + 1) >> 1) - 1);
	(CAN_UART)->BAUDCTRLA = bsel & 0xFF;
	(CAN_UART)->BAUDCTRLB = bsel >> 8;
}

Bool USB_callback_cdc_enable(uint8_t port)
{
	usb_cdc_enabled_bool[port] = true;
	return true;
}

void USB_callback_cdc_disable(uint8_t port)
{
	usb_cdc_enabled_bool[port] = false;
}

void USB_callback_rx_notify (uint8_t port) { //message received over USB
	uint8_t data;
	if (port == USB_CAN) {
		usart_putchar(CAN_UART, udi_cdc_multi_getc(port) ); //USB pass through
		
	}else if (port == USB_CMD) {
		while (udi_cdc_multi_is_rx_ready(port)) {  //is there data
			data = udi_cdc_multi_getc(port);	//get 1 char of data
			if (!udi_cdc_multi_is_tx_ready(port)) {
				udi_cdc_multi_signal_overrun(port);
			}else{ 
				udi_cdc_multi_putc(port, data);	//push char to loop back
			}
			if ( (data >= 0x20 && data <= 0x7F) || data == '\r' || data == '\n' || data == 8 ) {
				if (data == '\r' || data == '\n') { //if carriage return, run the menu
					menu_main();
					return;
				} else { //was a standard character that should be stored in the buffer
					menu_add_to_command(data);
				}
			}
		}
	}else if (port == USB_ACL) { //loop back
		while (udi_cdc_multi_is_rx_ready(port)) {  //is there data
			data = udi_cdc_multi_getc(port);	//get all the data
			udi_cdc_multi_putc(port, data); //loop back
			if (data == 'u' || data == 'U') {
					reset_trigger_USB();
			} else if (data == 'r' || data == 'R') {
					reset_trigger_SYSTEM();
			}
			
		}
	}
}
			
void USB_callback_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) {
		// Host terminal has open COM
		if (port == USB_CAN) {

		}else if (port == USB_CMD) {
			menu_send_n_st();
		}else if (port == USB_ACL) {

		}
	} else {
		// Host terminal has close COM
		if (port == USB_CAN) { 

		}else if (port == USB_CMD) {

		}else if (port == USB_ACL) {

		}
	}
	
}

Bool USB_port_is_active(uint8_t port) {
	return usb_cdc_enabled_bool[port];
}

void USB_send_string(uint8_t port, char * buffer) {	//send buffer
	int msg_l = strlen(buffer);
	int i = 0;
	while (i < msg_l) {
		USB_send_char(port, buffer[i]);
		i++;
	}	
}

void USB_send_char(uint8_t port, char value) {	//send buffer
	if (!udi_cdc_multi_is_tx_ready(port)) {
		udi_cdc_multi_signal_overrun(port);
	}else{
		udi_cdc_multi_putc(port, value);
	}
}

Bool USB_vbus_mount (void) {
	static Bool last;
	if (ioport_get_pin_level(EXT1_PIN_HUB_STATUS) == true)
	{	//pin is high, usb is mounted
		if (!last) { //if it was low
			udc_attach();
			last = true;
		}
		} else { //is low now
		if (last) { //if it was high before
			udc_detach();
			last = false;
			//			CAN_uart_start(); //reset to defaults
		}
	}
	return last;
}
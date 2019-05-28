/*
 * file: V2X_uart.c
 *
 * Created: 2/12/2016 10:34:41 AM
 *
 *  Author: Jesse Banks
 */

#include "V2X.h"

ISR(CAN_RX_Vect) { //CAN uart has received a char
	uint8_t value = usart_getchar(CAN_UART);
	if (CAN_is_controlled() || CAN_is_snooping() ) {
		CAN_new_data(value);
	}
	if (USB_port_is_active(USB_CAN) == true){
		//host is on, send over USB
		USB_send_char(USB_CAN, value);
 	}
}

ISR(CAN_TX_Vect) { //CAN uart has an empty send buffer
	if ( CAN_is_controlled() || CAN_is_snooping() ) {
		//if V2X if talking to the STN chip, don't inject USB data
			CAN_send_data();
	// the USB->uart pass through happens at the USB receive			
	}
}

ISR(SIM_RX_Vect)
{
	GSM_new_data ( usart_getchar(SIM_UART) );
}

ISR(SIM_TX_Vect)
{
   GSM_send_data();
}
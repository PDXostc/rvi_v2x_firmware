/*
 * V2X_uart.c
 *
 * Created: 2/12/2016 10:34:41 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

void canbus_serial_routing(uint8_t source)
{
	gpio_set_pin_low(BUF0_PIN);
	gpio_set_pin_low(BUF1_PIN);
	if		(source == FTDI_ROUTING)	{gpio_set_pin_high(BUF1_PIN);}
	else  /*(source == AVR_ROUTING)*/	{gpio_set_pin_high(BUF0_PIN);}
}


/*
 * V2X_drivers.c
 *
 * Created: 11/24/2015 1:36:13 PM
 *  Author: jbanks2
 */ 

#include "V2X_drivers.h"

void toggleChargePump(void)
{
	if (ioport_get_pin_level(CHARGEPUMP_0_PIN)) {gpio_set_pin_low(CHARGEPUMP_0_PIN);}
	else										{gpio_set_pin_high(CHARGEPUMP_0_PIN);}
}

void canbusSerialRouting(uint8_t direction)
{
	gpio_set_pin_low(BUF0_PIN);
	gpio_set_pin_low(BUF1_PIN);
	if		(direction == BUFFER_FTDI_ROUTING)	{gpio_set_pin_high(BUF1_PIN);}
	else  /*(direction == BUFFER_AVR_ROUTING)*/	{gpio_set_pin_high(BUF0_PIN);}
}
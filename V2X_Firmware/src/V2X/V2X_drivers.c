/**
 * /file V2X_drivers.c
 *
 * /brief V2X board hardware driver functions
 *
 * Author: Jesse Banks (jbanks2)
 **/ 

#include "V2X.h"

void canbus_serial_routing(uint8_t source)
{
	gpio_set_pin_low(BUF0_PIN);
	gpio_set_pin_low(BUF1_PIN);
	if		(source == FTDI_ROUTING)	{gpio_set_pin_high(BUF1_PIN);}
	else  /*(source == AVR_ROUTING)*/	{gpio_set_pin_high(BUF0_PIN);}
}

void spi_start(void)
{
	spi_master_init(ACL_SPI);
	sr_device_conf.id = EXT1_PIN_SR_LATCH;
	acl_device_conf.id = ACL_CS;
	spi_master_setup_device(SR_SPI, &sr_device_conf, SPI_MODE_0, 5000000, 0);
	spi_master_setup_device(ACL_SPI, &acl_device_conf, SPI_MODE_3, 5000000, 0);
	spi_enable(ACL_SPI);
}

void shift_register_init(void)
{
	shift_register_clear();							// Clear shift register
	shift_register_latch();							// Latch all internal registers to output
	shift_register_state = STATE_DEFAULT_VALUE;		// set power state to default
	state_to_shift_register();						// cause shift register to update
}

void spi_write_read_packet (SPI_t* spi, uint8_t* data, uint8_t length)
{
	while (length--) {						//increment through buffer
		spi_write_single(spi, *data);		//send byte pointed by data 

		while (!spi_is_rx_full(spi)) {		//wait for send complete
		}
		
		spi_read_single(spi, data);			//read back SPI data into data array
		data++;								//index through data array
	}
}

void shift_register_latch(void)
{
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
	gpio_set_pin_high(EXT1_PIN_SR_LATCH);	//latch values to output buffer
	gpio_set_pin_low(EXT1_PIN_SR_LATCH);
}

void shift_register_clear(void)
{
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
	gpio_set_pin_low(EXT1_PIN_SR_CLEAR);	//clear all internal registers
	gpio_set_pin_high(EXT1_PIN_SR_CLEAR);
}

void state_to_shift_register(void) {  //uses shift_register_state to update the shift register
	uint8_t data[2];
	data[1] = shift_register_state & 0xff;
	data[0] = (shift_register_state >> 8) & 0xff; 
	spi_write_packet(SR_SPI, data, 2);
	shift_register_latch();
}

void turn_on (uint16_t pins_mask) {  //updates teh power state variable but does not update shift register
	shift_register_state |= pins_mask;
}

void turn_off(uint16_t pins_mask) {  //updates teh power state variable but does not update shift register
	shift_register_state &= ~(pins_mask);
}

uint8_t host_is_on (void)
{
	if (shift_register_state & (1<<ENABLE_5V0)) 
			{		return true;	}
	else	{		return false;	}
}

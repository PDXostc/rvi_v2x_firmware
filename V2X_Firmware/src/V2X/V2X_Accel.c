/*
 * Accelerometer.c
 *
 * Created: 2/9/2016 10:53:17 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

void accelerometer_init(void)
{
	if (read_id() == ADLX345_DEVID){
		uint8_t data[7];
	
		data[0] = ACL_command_builder(ACL_WRITE, false, ACL_MAP_BW_RATE);
		data[1] = ACL_RATE_100;	//100hz
		ACL_send_recv_data(data, 2);
	
		data[0] = ACL_command_builder(ACL_WRITE, false, ACL_MAP_DATA_FORMAT);
		data[1] = (1<<ACL_FULL_RES)|(1<<ACL_RANGE_L)|(1<<ACL_RANGE_H); //full resolution 16g mode
		ACL_send_recv_data(data, 2);
	}
	else{
		//lucy, you got some splain'n to do!
	}
}

uint8_t ACL_command_builder (uint8_t read_write, uint8_t multibyte, uint8_t address)
{	
	uint8_t status = 0x3F & address;							//mask out upper bits
	if (read_write == ACL_READ) {status |= (1<<ACL_RW_BIT);}	//is read or write?
	if (multibyte)				{status |= (1<<ACL_MB_BIT);}	//is multi byte?
	return status;	
}

void ACL_send_recv_data(uint8_t* data, uint8_t length)
{
	spi_select_device(ACL_SPI, &acl_device_conf);
	spi_write_read_packet(ACL_SPI, data, length);
	spi_deselect_device(ACL_SPI, &acl_device_conf);
}

uint8_t read_id (void)
{
	uint8_t data[2];
	data[0] = ACL_command_builder(ACL_READ, false, ACL_MAP_DEVID);
	ACL_send_recv_data(data, 2);
	return data[1];
}

void ACL_sample_on (void)
{
	uint8_t data[2];
	data[0] = ACL_command_builder(ACL_WRITE, false, ACL_MAP_POWER_CTL);
	data[1] = (1<<ACL_MEASURE);		//cause the device to become active
	ACL_send_recv_data(data, 2);
}

void ACL_sample_off (void)
{
	uint8_t data[2];
	data[0] = ACL_command_builder(ACL_WRITE, false, ACL_MAP_POWER_CTL);
	data[1] = (1<<ACL_SLEEP);		//cause the device to power down
	ACL_send_recv_data(data, 2);
}

void ACL_sample (uint8_t* data)
{
	data[0] = ACL_command_builder(ACL_READ, true, ACL_MAP_DATAX0);
	ACL_send_recv_data(data, 7);
}
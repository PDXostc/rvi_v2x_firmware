/*
 * Accelerometer.c
 *
 * Created: 2/9/2016 10:53:17 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

Bool ACL_is_sampling = false;
char wbuffer[30];

void ACL_init(void)
{
	if (read_id() == ADLX345_DEVID){
		uint8_t data = ACL_RATE_100;	//100hz
		ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_BW_RATE), &data, 2);
	
		data = (1<<ACL_FULL_RES)|(1<<ACL_RANGE_L)|(1<<ACL_RANGE_H); //full resolution 16g mode
		ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_DATA_FORMAT), &data, 2);
	}
	else{
		//lucy, you got some splain'n to do!
	}
}

uint8_t ACL_command_builder (uint8_t read_write, uint8_t multibyte, uint8_t address)
{	
	uint8_t status = 0x3F & address;							//mask out upper bits
	if (read_write == ACL_READ) {status |= (1<<ACL_RW_BIT);}	//is read or write?
	if (multibyte == ACL_MULTI)	{status |= (1<<ACL_MB_BIT);}	//is multi byte?
	return status;	
}

void ACL_send_recv_data(uint8_t cmd, uint8_t* data, uint8_t length)
{
	spi_select_device(ACL_SPI, &acl_device_conf);
	spi_write_read_packet(ACL_SPI, cmd, data, length);
	spi_deselect_device(ACL_SPI, &acl_device_conf);
}

uint8_t read_id (void)
{
	uint8_t data;
	ACL_send_recv_data(ACL_command_builder(ACL_READ, ACL_SINGLE, ACL_MAP_DEVID), &data, 2);
	return data;
}

void ACL_sample_on (void)
{
	uint8_t data = (1<<ACL_MEASURE);
	ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_POWER_CTL), &data, 2);
	ACL_is_sampling = true;
}

void ACL_sample_off (void)
{
	uint8_t data = (1<<ACL_SLEEP);
	ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_POWER_CTL), &data, 2);
	ACL_is_sampling = false;
}

void ACL_sample (uint8_t* data)
{
	ACL_send_recv_data(ACL_command_builder(ACL_READ, ACL_MULTI, ACL_MAP_DATAX0), data, 7);
}

Bool ACL_sampling(void) {
	return 	ACL_is_sampling;
}

void ACL_data_to_string(uint8_t * data, char * buffer) {
	uint16_t data16;
	buffer[0] = '\0';
	strcat(buffer, "XYZT: ");  //create starting string
	for (int k = 0; k < 3; k++){  //convert and add x,y,z data, cat into buffer
		data16 = (data[2*k+1] << 8) | data[2*k];
		itoa(data16, wbuffer, 10);
		strcat(buffer, wbuffer);
		strcat(buffer, ", ");
	}
	itoa(udd_get_frame_number(), wbuffer, 10); //add timestamp
	strcat(buffer, wbuffer);
	strcat(buffer, "\r\n");		//end line
}
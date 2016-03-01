/*
 * Accelerometer.c
 *
 * Created: 2/9/2016 10:53:17 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

// Bool ACL_is_sampling = false; 
Bool new_config = true;
Bool new_offset = false;
char wbuffer[30];
uint8_t last_sample[6];
uint8_t last_rate = ACL_RATE_100;	//100hz
uint8_t last_data_format = (1<<ACL_RANGE_L)|(1<<ACL_RANGE_H)|(1<<ACL_FULL_RES);
uint8_t last_power = (1<<ACL_SLEEP);
uint8_t offsets[3] = {0,0,0};

void ACL_init(void) {
	if (read_id() == ADLX345_DEVID){
		last_rate = ACL_RATE_100;	//100hz
		last_data_format = (1<<ACL_RANGE_L)|(1<<ACL_RANGE_H)|(1<<ACL_FULL_RES);
		last_power = (1<<ACL_SLEEP);
		for (int i = 0; i < 3; i++) {
			offsets[i] = 0;
		}
		new_config = true;
		new_offset = true;
	}
	else{
		//lucy, you got some splain'n to do!
	}
}

uint8_t ACL_command_builder (uint8_t read_write, uint8_t multibyte, uint8_t address) {	
	uint8_t status = 0x3F & address;							//mask out upper bits
	if (read_write == ACL_READ) {status |= (1<<ACL_RW_BIT);}	//is read or write?
	if (multibyte == ACL_MULTI)	{status |= (1<<ACL_MB_BIT);}	//is multi byte?
	return status;	
}

void ACL_send_recv_data(uint8_t cmd, uint8_t* data, uint8_t length) {
	spi_select_device(ACL_SPI, &acl_device_conf);
	spi_write_read_packet(ACL_SPI, cmd, data, length);
	spi_deselect_device(ACL_SPI, &acl_device_conf);
}

uint8_t read_id (void) {		//special case where samples are not happening
	uint8_t data;
	ACL_send_recv_data(ACL_command_builder(ACL_READ, ACL_SINGLE, ACL_MAP_DEVID), &data, 2);
	return data;
}

void ACL_take_sample (uint8_t * data) {
	if (new_config) { //if new config is waiting
		ACL_send_configuration();
	}
	if (new_offset) { //if new offset is waiting
		ACL_send_offset();
	}
	if (ACL_sampling()) {
		ACL_send_recv_data(ACL_command_builder(ACL_READ, ACL_MULTI, ACL_MAP_DATAX0), data, 7);
		for (int i = 0; i < 6; i++) {
			last_sample[i] = data[i];
		}
	}
}

void ACL_get_last_sample (uint8_t * data) {
	for (int i = 0; i < 6; i++) {
		data[i] = last_sample[i];
	}
}

Bool ACL_sampling(void) {
	if (last_power == (1<<ACL_MEASURE)) {return true;
	} else {return false; }
}

void ACL_set_sample_on (void) {
	last_power = (1<<ACL_MEASURE);
	new_config = true; //state a new config is ready
}

void ACL_set_sample_off (void) {
	last_power = (1<<ACL_SLEEP); //set power state holder
	new_config = true; //state a new config is ready
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
	strcat(buffer, "\r");		//end line
}


void ACL_set_range(uint8_t range) {
	last_data_format &= ~((1<<ACL_RANGE_L)|(1<<ACL_RANGE_H)); // save existing last range
	switch (range) {
		case ACL_2G_RANGE:
			//last_data_format |= (0<<ACL_RANGE_L)|(0<<ACL_RANGE_H); //full resolution 16g mode
			break;
		case ACL_4G_RANGE:
			last_data_format |= (1<<ACL_RANGE_L)|(0<<ACL_RANGE_H); //full resolution 16g mode
			break;
		case ACL_8G_RANGE:
			last_data_format |= (0<<ACL_RANGE_L)|(1<<ACL_RANGE_H); //full resolution 16g mode
			break;
		case ACL_16G_RANGE:
		default:
			last_data_format |= (1<<ACL_RANGE_L)|(1<<ACL_RANGE_H); //full resolution 16g mode
			break;
	}	
	new_config = true; //signal update to config	
}

void ACL_set_full_resolution(Bool full) { //not 0 sets to "full range"
	if (full)  {last_data_format |= (1<<ACL_FULL_RES);
	} 	else   {last_data_format &= ~(1<<ACL_FULL_RES);}
	new_config = true; //signal update to config
}

void ACL_set_rate(uint8_t rate) {
	if (rate >= ACL_RATE_0_1 && rate <= ACL_RATE_3200) {
		last_rate = rate; //save value if in bounds
		new_config = true; //signal update to config
	}
}

void ACL_set_offset(uint8_t channel, uint8_t offset) {
	switch (channel) {
		case ACL_X_OFFSET:
			offsets[0] = offset;
			break;
		case ACL_Y_OFFSET:
			offsets[1] = offset;
			break;
		case ACL_Z_OFFSET:
			offsets[2] = offset;
			break;
	}
	new_offset = true;
}

void ACL_send_configuration(void) {
	ACL_send_power();
	ACL_send_data_format();
	ACL_send_rate();
	new_config = false; //clear the update bit
}

void ACL_send_power (void) {
	int temp = last_power;
	ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_POWER_CTL), &temp, 2);	
}

void ACL_send_data_format (void) {
	int temp = last_data_format;
	ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_DATA_FORMAT), &temp, 2);
}

void ACL_send_rate (void) {
	int temp = last_rate;
	ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_MAP_BW_RATE), &temp, 2);
}

void ACL_send_offset (void) {
	uint8_t temp = offsets[0];
	ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_X_OFFSET), &temp, 2);
	temp = offsets[1];
	ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_Y_OFFSET), &temp, 2);
	temp = offsets[2];
	ACL_send_recv_data(ACL_command_builder(ACL_WRITE, ACL_SINGLE, ACL_Z_OFFSET), &temp, 2);
	new_offset = false;
}
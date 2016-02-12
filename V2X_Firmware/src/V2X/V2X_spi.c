/*
 * V2X_spi.c
 *
 * Created: 2/12/2016 10:57:35 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

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

void spi_start(void)
{
	spi_master_init(ACL_SPI);
	sr_device_conf.id = EXT1_PIN_SR_LATCH;
	acl_device_conf.id = ACL_CS;
	spi_master_setup_device(SR_SPI, &sr_device_conf, SPI_MODE_0, 5000000, 0);
	spi_master_setup_device(ACL_SPI, &acl_device_conf, SPI_MODE_3, 5000000, 0);
	spi_enable(ACL_SPI);
}

/*
 * V2X_spi.h
 *
 * Created: 2/12/2016 10:57:47 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_SPI_H_
#define V2X_SPI_H_

/** 
 * @def SPI struct for shift register SPI device
 **/
struct spi_device sr_device_conf;
	
/** 
 * @def SPI struct for accelerometer SPI device
 **/
struct spi_device acl_device_conf;
	
/**
 * @def spi_start
 * @brief configures and enables the SPI peripheral
 **/
void spi_start(void);

/**
 * @def spi_write_read_packet
 * @brief sends a multi byte packet, returns the received bytes in the same data array
 * @param spi Base address of the SPI instance.
 * @param cmd the comand byte to send to the device
 * @param data The data byte array to be sent
 * @length size of data in bytes
 **/
void spi_write_read_packet (SPI_t* spi, uint8_t cmd, uint8_t* data, uint8_t length);

#endif /* V2X_SPI_H_ */
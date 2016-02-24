/*
 * Accelerometer.h
 *
 * Created: 2/9/2016 10:40:47 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_ACCEL_H_
#define V2X_ACCEL_H_

Bool ACL_is_sampling;

/**
 * @def acl_register_map
 * @brief register address definitions.
 **/	
enum acl_register_map {
	ACL_MAP_DEVID			= 0,  //Device ID       R/W default= 0
	ACL_MAP_THRESH_TAP		= 29, //Tap threshold       R/W default= 0
	ACL_MAP_OFSX			= 30, //X-axis offset       R/W default= 0
	ACL_MAP_OFSY			= 31, //Y-axis offset       R/W default= 0
	ACL_MAP_OFSZ			= 32, //Z-axis offset       R/W default= 0
	ACL_MAP_DUR				= 33, //Tap duration       R/W default= 0
	ACL_MAP_Latent			= 34, //Tap latency       R/W default= 0
	ACL_MAP_Window			= 35, //Tap window       R/W default= 0
	ACL_MAP_THRESH_ACT		= 36, //Activity threshold       R/W default= 0
	ACL_MAP_THRESH_INACT	= 37, //Inactivity threshold       R/W default= 0
	ACL_MAP_TIME_INACT		= 38, //Inactivity time       R/W default= 0
	ACL_MAP_ACT_INACT_CTL	= 39, //Axis enable control for activity and inactivity detection R/W default= 0
	ACL_MAP_THRESH_FF		= 40, //Free-fall threshold       R/W default= 0
	ACL_MAP_TIME_FF			= 41, //Free-fall time       R/W default= 0
	ACL_MAP_TAP_AXES		= 42, //Axis control for single tap/double tap   R default= 0
	ACL_MAP_ACT_TAP_STATUS	= 43, //Source of single tap/double tap    R/W default= 1010
	ACL_MAP_BW_RATE			= 44, //Data rate and power mode control   R/W default= 0
	ACL_MAP_POWER_CTL		= 45, //Power-saving features control      R/W default= 0
	ACL_MAP_INT_ENABLE		= 46, //Interrupt enable control      R/W default= 0
	ACL_MAP_INT_MAP			= 47, //Interrupt mapping control      R default= 10
	ACL_MAP_INT_SOURCE		= 48, //Source of interrupts      R/W default= 0
	ACL_MAP_DATA_FORMAT		= 49, //Data format control      R default= 0
	ACL_MAP_DATAX0			= 50, //X-Axis Data 0      R default= 0
	ACL_MAP_DATAX1			= 51, //X-Axis Data 1      R default= 0
	ACL_MAP_DATAY0			= 52, //Y-Axis Data 0      R default= 0
	ACL_MAP_DATAY1			= 53, //Y-Axis Data 1      R default= 0
	ACL_MAP_DATAZ0			= 54, //Z-Axis Data 0      R default= 0
	ACL_MAP_DATAZ1			= 55, //Z-Axis Data 1      R/W default= 0
	ACL_MAP_FIFO_CTL		= 56, //FIFO control       R default= 0
	ACL_MAP_FIFO_STATUS		= 57  //FIFO status        default=
	};

/**
 * @def ACL_command_tags
 * @brief command byte bit definitions.
 **/	
enum ACL_command_tags {
	ACL_WRITE				= 0,
	ACL_READ				= 1,
	ACL_MB_BIT				= 6,
	ACL_RW_BIT				= 7
	};

enum ACL_message_length {
	ACL_SINGLE,
	ACL_MULTI
	};
/**
 * @def ACL_sample_rate
 * @brief sample rate register bit definitions.
 **/	
enum ACL_sample_rate{
	ACL_RATE_3200 = 0x0F,
	ACL_RATE_1600 = 0x0E,
	ACL_RATE_800 = 0x0D,
	ACL_RATE_400 = 0x0C,
	ACL_RATE_200 = 0x0B,
	ACL_RATE_100 = 0x0A,
	ACL_RATE_50 = 0x09,
	ACL_RATE_25 = 0x08,
	ACL_RATE_12 = 0x07,
	ACL_RATE_6 = 0x06,
	ACL_RATE_3 = 0x05,
	ACL_RATE_1 = 0x04,
	ACL_RATE_0_78 = 0x03,
	ACL_RATE_0_39 = 0x02,
	ACL_RATE_0_2 = 0x01,
	ACL_RATE_0_1 = 0x00
	};
	
/**
 * @def ACL_power_bits
 * @brief power register bit definitions.
 **/	
enum ACL_power_bits{
	ACL_WAKE_L,
	ACL_WAKE_H,
	ACL_SLEEP,
	ACL_MEASURE,
	ACL_AUTO_SLEEP,
	ACL_LINK
	};

/**
 * @def ACL_data_format_bits
 * @brief data format register bit definitions.
 **/	
enum ACL_data_format_bits{
	ACL_RANGE_L,
	ACL_RANGE_H,
	ACL_JUSTIFY, // value of 1 is left, 0 is right
	ACL_FULL_RES,
	ACL_INTERUPT_INVERT,
	ACL_SPI_MODE,
	ACL_SELF_TEST
	};
	
#define ADLX345_DEVID 0xe5  //hardware specific device ID

/**
 * @def accelerometer_init
 * @brief initializes the accelerometer to default values, but does not start
 **/
void ACL_init(void);

/**
 * @def ACL_command_builder
 * @brief generates the command instruction sent to ACL
 * param is this a read or write command? ACL_READ, ACL_WRITE
 * param is it a multibyte command? true, false
 * param what ACL register to touch. acceptable names in acl_register_map
 **/
uint8_t ACL_command_builder (uint8_t read_write, uint8_t multibyte, uint8_t address);

/**
 * @def ACL_send_recv_data
 * @brief send or receive data from ACL device.
 * @param data is pointer to an array of size "length", write data is replaced with read data
 * @param length is 1 command + X data in length
 **/
void ACL_send_recv_data(uint8_t cmd, uint8_t* data, uint8_t length);

/**
 * @def read_id
 * @brief queries the ACL for a unique device ID
 **/
uint8_t read_id (void);

/**
 * @def ACL_sample_on
 * @brief Starts motion sampling, puts ADX into sample mode.
 **/
void ACL_sample_on (void);

/**
 * @def ACL_sample_off
 * @brief Stops motion sampling, puts ADX into sleep mode.
 **/
void ACL_sample_off (void);

/**
 * @def ACL_sample
 * @brief pulls all 6 samples from the accelerometer XH:XL, YH:YL, ZH:ZL
 * @param data is a pointer to array uint8_t[7], pass by reference, {ZH, ZL, YH, YL, XH, XL, CMD}={6:0}
 **/
void ACL_sample (uint8_t* data);

/**
 * @def ACL_sampling
 * @brief reports true if the accelerometer has been enabled to sample and stream
 **/
Bool ACL_sampling(void);

/**
 * @def ACL_data_to_string
 * @brief converts the raw data from the accelerometer to a serial string for sending to host
 * @param data 6 element buffer pulled from ADXL
 * @param buffer the buffer for building the string in
 **/
void ACL_data_to_string(uint8_t * data, char * buffer);

#endif /* V2X_ACCEL_H_ */
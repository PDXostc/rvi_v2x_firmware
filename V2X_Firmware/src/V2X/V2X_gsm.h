/*
 * V2X_gsm.h
 *
 * Created: 2/12/2016 11:01:18 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_GSM_H_
#define V2X_GSM_H_

/**
 * @def simcard_status
 * @brief detection of SIMCARD by socket switch 
 * returns "true" if SIMCARD installed 
 **/
inline static bool simcard_status(void) {return 1-ioport_get_pin_level(SW1_PIN);}
	/**
 * @def sim_power_status
 * @brief detection of SIMCARD by socket switch 
 * returns "true" if SIMCARD installed 
 **/
inline static bool sim_power_status(void) {return ioport_get_pin_level(EXT1_PIN_SIM_PWR);}
/**
 * @def sim_net_status
 * @brief detection of SIMCARD by socket switch 
 * returns "true" if SIMCARD installed 
 **/
inline static bool sim_net_status(void) {return ioport_get_pin_level(EXT1_PIN_SIM_NETWORK);}

/**
 * @def GSM_set_tx_int
 * @brief sets the interrupts active
 */
void GSM_set_tx_int(void);

/**
 * @def GSM_clear_tx_int
 * @brief sets the interrupts inactive
 */
void GSM_clear_tx_int(void);

/**
 * @def GSM_add_to_buffer
 * @brief adds the input char to the selected buffer
 * @param buffer_select (0,1) input/output
 * @param value what to add to the buffer
 */
void GSM_add_to_buffer(uint8_t buffer_select, char value);

/**
 * @def GSM_bytes_to_send
 * @brief querys the number of bytes remaining to be sent
 * @param buffer_select (0,1) input/output
 * @retval bytes remaining to be sent
 */
int  GSM_bytes_to_send (uint8_t buffer_select);

/**
 * @def GSM_next_byte
 * @brief gets the next char and increments the pointer
 * @param buffer_select (0,1) input/output
 * @retval next char in the buffer
 */
char GSM_next_byte (uint8_t buffer_select);

/**
 * @def GSM_process_buffer
 * @brief if the buffer has data it set sending in motion
 * @param buffer_select (0,1) input/output
 */
void GSM_process_buffer (uint8_t buffer_select);

/**
 * @def GSM_purge_buffer
 * @brief empties the buffer to 0 and resets pointers
 * @param buffer_select (0,1) input/output
 */
void GSM_purge_buffer(uint8_t buffer_select);

/**
 * @def GSM_usart_init
 * @brief configure the usart port used by the GSM device, active RX
 */
void GSM_usart_init (void);

/**
 * @def buffer_selection
 * @brief switch for selecting in and out buffers
 */
enum buffer_selection {
	BUFFER_IN = 0,
	BUFFER_OUT
	};
	
#endif /* V2X_GSM_H_ */
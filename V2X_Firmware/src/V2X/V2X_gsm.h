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
//void GSM_add_to_buffer(uint8_t buffer_select, char value);
//void GSM_add_string_to_buffer_P(uint8_t buffer_select, char * to_add);
/**
 * @def GSM_bytes_to_send
 * @brief querys the number of bytes remaining to be sent
 * @param buffer_select (0,1) input/output
 * @retval bytes remaining to be sent
 */
//int  GSM_bytes_to_send (uint8_t buffer_select);

/**
 * @def GSM_next_byte
 * @brief gets the next char and increments the pointer
 * @param buffer_select (0,1) input/output
 * @retval next char in the buffer
 */
//char GSM_next_byte (uint8_t buffer_select);

/**
 * @def GSM_process_buffer
 * @brief if the buffer has data it set sending in motion
 * @param buffer_select (0,1) input/output
 */
void GSM_process_buffer (void);
void GSM_add_string_to_buffer(Bool in_out, char * to_add);
void GSM_mark_for_processing(Bool in_out);
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

	
enum GSM_sequence_states {
	GSM_state_idle = 0,
	GSM_state_start,
	GSM_state_init_SMS,
	GSM_state_time_get,
	GSM_state_wake_host
	};
	
enum GSM_subsequence_states {
	GSM_subssequence_1 = 0,
	GSM_subssequence_2,
	GSM_subssequence_3,
	GSM_subssequence_4,
	GSM_subssequence_5,
	GSM_subssequence_6,
	GSM_subssequence_7,
	GSM_subssequence_8,
	GSM_subssequence_9,
	GSM_subssequence_10,
	GSM_subssequence_FAIL
	};
	
void GSM_control(char * responce_buffer);
void GSM_control_start(char * responce_buffer);
void GSM_start_sleep (void);
void show_buffer(char * buffer);


#endif /* V2X_GSM_H_ */
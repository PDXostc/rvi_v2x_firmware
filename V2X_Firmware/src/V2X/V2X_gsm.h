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
 * @def GSM_process_buffer
 * @brief if the buffer has data it set sending in motion
 * @param buffer_select (0,1) input/output
 */
void GSM_process_buffer (void);


/**
 * @def GSM_add_string_to_buffer
 * @brief adds a passed char* string to the active buffer
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @param *to_add char string to add the buffer
*/void GSM_add_string_to_buffer(Bool in_out, char * to_add);

/**
 * @def GSM_mark_for_processing
 * @brief changes roles of A/B buffers - in/active
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
*/
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
 * @def GSM_sequence_states
 * @brief switch for selecting control states
 */
enum GSM_sequence_states {
	GSM_state_idle = 0,
	GSM_state_check,
	GSM_state_start,
	GSM_state_init_SMS,
	GSM_state_time_get,
	GSM_state_wake_host
	};
	
/**
 * @def GSM_sequence_states
 * @brief generic switch for moving through control substates
 */
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
	
/**
 * @def GSM_control
 * @brief head of the controller sequencer, sends calls into corect module
 * @param *responce_buffer pointer to the buffer to be analysed
 */
void GSM_control(char * responce_buffer);

/**
 * @def GSM_control_check
 * @brief SIM5320 startup pre-start module, set power check for responce
 * @param *responce_buffer pointer to the buffer to be analysed
 */
void GSM_control_check (char * responce_buffer);

/**
 * @def GSM_control_start
 * @brief SIM5320 startup start module, stop echo, check SIM model, capture IMEI number
 * @param *responce_buffer pointer to the buffer to be analysed
 */
void GSM_control_start(char * responce_buffer);

/**
 * @def GSM_begin_init
 * @brief sets up conditions to begin interaction with GSM
 */
void GSM_begin_init (void);

/**
 * @def show_buffer
 * @brief prints what it finds to the CMD USB for debug purposes
 * @param *buffer pointer to the buffer to be sent to user
 */
void show_buffer(char * buffer);
void GSM_time_job (void);
void GSM_time_sync  (char * responce_buffer);
void GSM_parse_gps_info (char * responce_buffer);
//void show_indexes (int start_i, int end_i);
void job_set_timeout (int span);
Bool job_check_timeout(void);
void job_clear_timeout (void);
void job_check_fail (void);

#endif /* V2X_GSM_H_ */
/*
 * V2X_can.h
 *
 * Created: 3/10/2016 12:25:21 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_CAN_H_
#define V2X_CAN_H_

/**
 * @def CAN_sequence_states
 * @brief switch for selecting control states
 */
enum CAN_sequence_states {
	CAN_state_idle = 0,
	CAN_state_power,
	CAN_state_EE,
	};
	
/**
 * @def CAN_subsequence_states
 * @brief generic switch for moving through control substates
 */
enum CAN_subsequence_states {
	CAN_subsequence_1 = 0,
	CAN_subsequence_2,
	CAN_subsequence_3,
	CAN_subsequence_4,
	CAN_subsequence_5,
	CAN_subsequence_6,
	CAN_subsequence_7,
	CAN_subsequence_8,
	CAN_subsequence_9,
	CAN_subsequence_10,
	CAN_subsequence_COMPLETE,
	CAN_subsequence_FAIL
	};

/**
 * @def CAN_uart_start
 * @brief configure the usart port used by the CAN device, active RX
 */
void CAN_uart_start (void);

/**
 * @def CAN_uart_stop
 * @brief configure the usart port used by the GSM device disabled
 */
void CAN_uart_stop (void);

/**
 * @def CAN_set_tx_int
 * @brief sets the interrupts active
 */
void CAN_set_tx_int(void);

/**
 * @def CAN_clear_tx_int
 * @brief sets the interrupts inactive
 */
void CAN_clear_tx_int(void);

/**
 * @def CAN_add_to_buffer
 * @brief adds the input char to the selected buffer
 * @param buffer_select (0,1) input/output
 * @param value what to add to the buffer
 */
void CAN_add_to_buffer(uint8_t buffer_select, char value);

/**
 * @def CAN_bytes_to_send
 * @brief querys the number of bytes remaining to be sent
 * @param buffer_select (0,1) input/output
 * @retval bytes remaining to be sent
 */
int  CAN_bytes_to_send (uint8_t buffer_select);

/**
 * @def CAN_next_byte
 * @brief gets the next char and increments the pointer
 * @param buffer_select (0,1) input/output
 * @retval next char in the buffer
 */
char CAN_next_byte (uint8_t buffer_select);

/**
 * @def CAN_process_buffer
 * @brief if the buffer has data it set sending in motion
 * @param buffer_select (0,1) input/output
 */
void CAN_process_buffer (void);

/**
 * @def CAN_purge_buffer
 * @brief empties the buffer to 0 and resets pointers
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
*/
void CAN_purge_buffer(uint8_t buffer_select);

/**
 * @def CAN_mark_for_processing
 * @brief swaps the roles of the A/B buffers, starts sending routines
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
*/
void CAN_mark_for_processing (Bool in_out);

/**
 * @def CAN_add_string_to_buffer
 * @brief adds the passed-in string to the active buffer, in/out
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @param *to_add pointer to the string that should be added
*/
void CAN_add_string_to_buffer(Bool in_out, char * to_add);


/**
 * @def CAN_control
 * @brief interaction control sequencer, host asleep.
 * @param *responce_buffer pointer to the buffer to be analysed
*/
void CAN_control (char * responce_buffer);

/**
 * @def CAN_control_fail
 * @brief puts the CAN control sequence in a fail state due to timeout
*/
void CAN_control_fail (void);

/**
 * @def CAN_mark_as_sent
 * @brief clears the loaded bit in the can output buffer
*/
void CAN_mark_as_sent(void);

/**
 * @def CAN_elm_init
 * @brief Starts a control sequence job that initializes the ELM
*/
void CAN_elm_init (void);

/**
 * @def CAN_control_init
 * @brief control processor for the init sequence
 * @param *responce_buffer pointer to the buffer to be analysed
*/
void CAN_control_init (char * responce_buffer);

/**
 * @def CAN_is_controlled
 * @brief indicates the ELM is under transaction with controller
*/
Bool CAN_is_controlled(void);

/**
 * @def CAN_new_data
 * @brief called by serial ISR to store data to the inpout buffer
 * @param value the value to store in buffer
*/
void CAN_new_data (uint8_t value);

/**
 * @def CAN_send_data
 * @brief routine called by serial Tx ISR for sending output buffer data
*/
void CAN_send_data (void);

/**
 * @def CAN_start_snoop
 * @brief called by the VXCX menu to pass/listen CAN commands in CMD
*/
void CAN_start_snoop (void);

/**
 * @def CAN_stop_snoop
 * @brief stops listening of CAN by CMD
*/
void CAN_stop_snoop (void);

/**
 * @def CAN_is_snooping
 * @brief used to determine the snoop state
*/
Bool CAN_is_snooping(void);

/**
 * @def CAN_ee_sequence
 * @brief used by can_control to iterate through the EE commands
*/
void CAN_ee_sequence(char * responce_buffer);

/**
 * @def CAN_EE_start
 * @brief kicks off the ee sequence job
*/
void CAN_EE_start (void);

/**
 * @def CAN_find_message
 * @brief extracts sections of the EE string
*/
Bool CAN_find_message (char * buffer, uint8_t index);

/**
 * @def CAN_get_sequence_state
 * @brief gets value of CAN_sequence_state variable
 * @return value of CAN_sequence_state variable
 */
uint8_t CAN_get_sequence_state();

/**
 * @def CAN_get_subsequence_state
 * @brief gets value of CAN_subsequence_state variable
 * @return value of CAN_subsequence_state variable
 */
uint8_t CAN_get_subsequence_state();

#endif /* V2X_CAN_H_ */
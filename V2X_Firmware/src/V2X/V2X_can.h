/*
 * V2X_can.h
 *
 * Created: 3/10/2016 12:25:21 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_CAN_H_
#define V2X_CAN_H_

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
void CAN_process_buffer ();

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
 * @def CAN_power_off
 * @brief disables the ELM with hardware reset pin
*/
void CAN_power_off (void);

/**
 * @def CAN_power_on
 * @brief enables the ELM with hardware reset pin
*/
void CAN_power_on (void);

/**
 * @def CAN_restart
 * @brief disables then enables the ELM with hardware reset pin and sleep
*/
void CAN_restart (void);

#endif /* V2X_CAN_H_ */
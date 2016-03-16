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
 * @param buffer_select (0,1) input/output
*/
void CAN_purge_buffer(uint8_t buffer_select);

void CAN_mark_for_processing (Bool in_out);
void CAN_add_string_to_buffer(Bool in_out, char * to_add);
#endif /* V2X_CAN_H_ */
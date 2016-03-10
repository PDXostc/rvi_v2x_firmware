/*
 * V2X_can.h
 *
 * Created: 3/10/2016 12:25:21 PM
 *  Author: jbanks2
 */ 


#ifndef V2X_CAN_H_
#define V2X_CAN_H_

void CAN_uart_start (void);
void CAN_uart_stop (void);
void CAN_set_tx_int(void);
void CAN_clear_tx_int(void);
void CAN_add_to_buffer(uint8_t buffer_select, char value);
int  CAN_bytes_to_send (uint8_t buffer_select);
char CAN_next_byte (uint8_t buffer_select);
void CAN_process_buffer (uint8_t buffer_select);
void CAN_purge_buffer(uint8_t buffer_select);
void CAN_set_tx_int(void);
void CAN_clear_tx_int(void);

#endif /* V2X_CAN_H_ */
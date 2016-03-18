/*
 * V2X_command.h
 *
 * Created: 3/15/2016 9:47:11 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_COMMAND_H_
#define V2X_COMMAND_H_

/**
 * @def buff_t
 * @brief buffer struct for the double buffering system
 **/
typedef struct buff_t {
	Bool active_in; //being active indicates where new bytes are stored
	Bool active_out; //being inactive indicates send/processing 
	char input_a[40]; //input buffer A
	char input_b[40]; //input buffer B
	char output_a[20]; //output buffer A
	char output_b[20]; //output buffer B
	uint8_t in_store_a;  //input storage pointer for A
	uint8_t in_store_b;  //input storage pointer for B
	uint8_t out_store_a; //output storage pointer for A
	uint8_t out_store_b; //output storage pointer for B
	uint8_t in_proc_a;   //input processing pointer for A
	uint8_t in_proc_b;	 //input processing pointer for B
	uint8_t out_proc_a;	 //output processing pointer for A
	uint8_t out_proc_b;	 //output processing pointer for B
	} buff;

/**
 * @def buffer_selection
 * @brief buffer deffinition for the double buffering system
 **/
enum buffer_selection {
	BUFFER_IN = 0,
	BUFFER_OUT
};

/**
 * @def buffer_switch
 * @brief buffer deffinition for the double buffering system
 **/
enum buffer_switch {
	BUFFER_A = 0,
	BUFFER_B
};

/**
 * @def CTL_add_to_buffer
 * @brief adds a single passed char value to the active buffer
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @param value char to add the string
*/
void CTL_add_to_buffer(buff * buffer, Bool in_out, char value);

/**
 * @def CTL_add_string_to_buffer_P
 * @brief adds a PGM stored passed char* string to the active buffer
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @param *to_add PSTR("...") stored char string to add the buffer
*/
void CTL_add_string_to_buffer_P(buff * buffer, Bool in_out, const char * to_add);

/**
 * @def CTL_add_string_to_buffer
 * @brief adds a passed char* string to the active buffer
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @param *to_add char string to add the buffer
*/
void CTL_add_string_to_buffer(buff * buffer, Bool in_out, char * to_add);

/**
 * @def CTL_bytes_to_send
 * @brief numbers of bytes remaining to be sent from the inactive buffer
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @reval length of inactive buffer minus current recall position
*/
int CTL_bytes_to_send (buff * buffer, Bool in_out);

/**
 * @def CTL_next_byte
 * @brief gets char from inactive buffer, increments recall position
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @reval returns inactive_buffer[recall_possition]
*/
char CTL_next_byte (buff * buffer, Bool in_out);

/**
 * @def CTL_purge_buffer
 * @brief clears buffer, reset message lenth and recall pointer
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
*/
void CTL_purge_buffer(buff * buffer, Bool in_out);

/**
 * @def CTL_mark_for_processing
 * @brief changes roles of A/B buffers - in/active
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
*/
void CTL_mark_for_processing(buff * buffer, Bool in_out);

/**
 * @def CTL_ptr_to_proc_buffer
 * @brief returns a pointer to the buffer holding the string to be processed (inactive)
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @retval char * to add the inactive string
*/
char * CTL_ptr_to_proc_buffer(buff * buffer, Bool in_out);

/**
 * @def clear_buffer
 * @brief resets the buffer to all '\0'
 * @param buffer pointer to the beginning of the buffer
 **/
void clear_buffer(char * buffer);

#endif /* V2X_COMMAND_H_ */
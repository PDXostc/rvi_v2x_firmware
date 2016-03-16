/*
 * V2X_command.h
 *
 * Created: 3/15/2016 9:47:11 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_COMMAND_H_
#define V2X_COMMAND_H_

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

enum buffer_selection {
	BUFFER_IN = 0,
	BUFFER_OUT
};

enum buffer_switch {
	BUFFER_A = 0,
	BUFFER_B
};

void CTL_add_to_buffer(buff * buffer, Bool in_out, char value);
void CTL_add_string_to_buffer_P(buff * buffer, Bool in_out, const char * to_add);
void CTL_add_string_to_buffer(buff * buffer, Bool in_out, char * to_add);
int CTL_bytes_to_send (buff * buffer, Bool in_out);
char CTL_next_byte (buff * buffer, Bool in_out);
void CTL_purge_buffer(buff * buffer, Bool in_out);
void CTL_mark_for_processing(buff * buffer, Bool in_out);
char * CTL_ptr_to_proc_buffer(buff * buffer, Bool in_out);
/**
 * @def clear_CMD_buffer
 * @brief resets the command buffer to '\0'
 * @param CMD_buffer pointer to the beginning of the buffer
 **/
void clear_buffer(char * CMD_buffer);

#endif /* V2X_COMMAND_H_ */
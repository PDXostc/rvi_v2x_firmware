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
	uint8_t input_index;			//input buffer index at most recently received data 
	uint8_t input_proc_index;		//input buffer index at beginning of unprocessed data
	uint8_t output_proc_index;		//output processing index
	
	char input_buf[200];			//input circular buffer
	char input_proc_buf[100];		//input processing buffer
	char output_proc_buf[20];		//output processing buffer
	
	Bool input_proc_flag;			//the input buffer is ready to be searched for a string
	Bool input_proc_loaded;			//the input process buffer is loaded for analysis
	Bool output_proc_active_flag;	//the output buffer is being used
	Bool output_proc_loaded;		//the output buffer is primed and ready to send.
	
	} buff;

/**
 * @def buffer_selection
 * @brief buffer definition for the double buffering system
 **/
enum buffer_selection {
	BUFFER_IN = 0,
	BUFFER_OUT
};

/**
 * @def buffer_switch
 * @brief buffer definition for the double buffering system
 **/
enum buffer_switch {
	BUFFER_A = 0,
	BUFFER_B
};

/**
 * @def system_switch
 * @brief system definition for the double buffering system
 **/
enum system_switch {
	SYS_CAN = 0,	//used for direct control of CAN
	SYS_GSM,		//used for direct control of GSM
	SYS_CAN_CTL,	//used for CTL passthrough of CAN
	SYS_CTL,		//stand alone sequence jobs, wake host, etc.
	SYS_PWR,
	SYS_CAR_STATE_CHECK,
	SYS_NUM
};

/**
 * @def CTL_add_to_buffer
 * @brief adds a single passed char value to the active buffer
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @param value char to add the string
*/
void CTL_add_to_buffer (buff * buffer, Bool in_out, char value);

/**
 * @def CTL_add_string_to_buffer_P
 * @brief adds a PGM stored passed char* string to the active buffer
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @param *to_add PSTR("...") stored char string to add the buffer
*/
void CTL_add_string_to_buffer_P (buff * buffer, Bool in_out, const char * to_add);

/**
 * @def CTL_add_string_to_buffer
 * @brief adds a passed char* string to the active buffer
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @param *to_add char string to add the buffer
*/
void CTL_add_string_to_buffer (buff * buffer, Bool in_out, char * to_add);

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
void CTL_purge_buffer (buff * buffer, Bool in_out);

/**
 * @def CTL_mark_for_processing
 * @brief changes roles of A/B buffers - in/active
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
*/
void CTL_mark_for_processing (buff * buffer, Bool in_out);

/**
 * @def CTL_ptr_to_proc_buffer
 * @brief returns a pointer to the buffer holding the string to be processed (inactive)
 * @param *buffer pointer to the buffer struct buff_t
 * @param buffer_select "BUFFER_IN", "BUFFER_OUT"
 * @retval char * to add the inactive string
*/
char * CTL_ptr_to_proc_buffer (buff * buffer, Bool in_out);

/**
 * @def clear_buffer
 * @brief resets the buffer to all '\0'
 * @param buffer pointer to the beginning of the buffer
 **/
void clear_buffer (char * buffer);

/**
 * @def CTL_copy_to_proc
 * @brief moves oldest string in circular input buffer into proc buffer
 * @param *buffer pointer to the buffer struct buff_t
 **/
void CTL_copy_to_proc (buff * buffer);

/**
 * @def CTL_input_index_check
 * @brief looks to see if the buffer size has been exceeded
 * @param *buffer pointer to the buffer struct buff_t
 **/
void CTL_input_index_check (buff * buffer);

/**
 * @def CTL_input_proc_index_check
 * @brief looks to see if the buffer size has been exceeded
 * @param *buffer pointer to the buffer struct buff_t
 **/
void CTL_input_proc_index_check (buff * buffer);

/**
 * @def job_coordinator
 * @brief uses the RTC to launch chron jobs
 **/
void job_coordinator (void);

/**
 * @def job_timeout_init
 * @brief clears all timeout 
 **/
void job_timeout_init (void);

/**
 * @def job_set_timeout
 * @brief sets a new timeout time and enables checking of timeouts
 * @param system - SYS_CAN or SYS_GPS system
 * @param span - number of seconds until timeout
 **/
void job_set_timeout (uint8_t system, uint16_t span);

/**
 * @def job_check_timeout
 * @brief check of timeout of system and return if expired
 * @param system - SYS_CAN or SYS_GPS system
 * @retval true if timer has expired 
 **/
Bool job_check_timeout(uint8_t system);

/**
 * @def job_clear_timeout
 * @brief disables checking of timeouts
 * @param system - SYS_CAN or SYS_GPS system
 **/
void job_clear_timeout (uint8_t system);

/**
 * @def job_check_fail
 * @brief check of timeout of system, launch fail recovery if expired
 * @param system - SYS_CAN or SYS_GPS system
 **/
void job_check_fail (uint8_t system);

#endif /* V2X_COMMAND_H_ */
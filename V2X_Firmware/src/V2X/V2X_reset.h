/*
 * V2X_reset.h
 *
 * Created: 9/20/2017 10:51:08 PM
 *  Author: Jesse Banks
 */ 


#ifndef V2X_RESET_H_
#define V2X_RESET_H_

/**
 * @def reset_flag_defs
 * @param used to track what system is being serviced
 **/
enum reset_flag_defs {
	RESET_NONE = 0,
	RESET_SYSTEM,
	RESET_USB,
	RESET_CAN,
	RESET_GSM
};

/**
 * @def reset_processor
 * @brief check reset flags and performs actions needed
 **/
void reset_processor(void);

/**
 * @def reset_trigger_USB
 * @brief marks reset flag for USB 
 **/
void reset_trigger_USB (void);

/**
 * @def reset_trigger_SYSTEM
 * @brief marks reset flag for the AVR
 **/
void reset_trigger_SYSTEM (void);

/**
 * @def reset_trigger_CAN
 * @brief marks reset flag for ELM
 **/
void reset_trigger_CAN (void);

/**
 * @def reset_trigger_SIM
 * @brief marks reset flag for GSM
 **/
void reset_trigger_GSM (void);

/**
 * @def 
 * @brief 
 * @param 
 * @retval
 **/
#endif /* V2X_RESET_H_ */
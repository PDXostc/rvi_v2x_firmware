/**
 * /file V2X_init.h
 *
 * /brief V2X board initialization definitions
 *
 * Author: Jesse Banks (jbanks2)
 **/
#ifndef V2X_INIT_H_
#define V2X_INIT_H_

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
 * @def pin_init
 * @brief Whole chip pin initialization (mode, state)
 **/
void pin_init(void);

/**
 * @def v2x_board_init
 * @brief Whole board initialization (mode, state)
 **/
void v2x_board_init(void);

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
#endif /* V2X_INIT_H_ */

/**
 * /file V2X_init.h
 *
 * /brief V2X board initialization definitions
 *
 * Author: Jesse Banks (jbanks2)
 **/
#ifndef V2X_INIT_H_
#define V2X_INIT_H_

enum reset_flag_defs {
	RESET_NONE = 0,
	RESET_SYSTEM,
	RESET_USB,
	RESET_CAN,
	RESET_SIM
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

void reset_trigger_USB (void);
void reset_trigger_SYSTEM (void);
void reset_trigger_CAN (void);
void reset_trigger_SIM (void);
void reset_processor(void);

#endif /* V2X_INIT_H_ */

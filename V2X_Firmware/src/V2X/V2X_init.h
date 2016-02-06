/**
 * /file V2X_init.h
 *
 * /brief V2X board initialization definitions
 *
 * Author: Jesse Banks (jbanks2)
 **/
#ifndef V2X_INIT_H_
#define V2X_INIT_H_

#include "V2X_board.h"
#include "V2X_drivers.h"

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

#endif /* V2X_INIT_H_ */

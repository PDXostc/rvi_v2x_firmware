/**
 * \file user_board.h
 *
 *  Author: Jesse Banks
 *
 * \brief User board external clock source definition
 *
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

// External oscillator settings.

// External oscillator frequency
#define BOARD_XOSC_HZ          32768

// External oscillator type.
// 32.768 kHz resonator on TOSC
#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ

// External oscillator startup time
#define BOARD_XOSC_STARTUP_US  500000

#define V2X_OSC_DEF

#endif // USER_BOARD_H

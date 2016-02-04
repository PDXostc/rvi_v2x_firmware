/*
 * IncFile1.h
 *
 * Created: 10/15/2015 2:48:04 PM
 *  Author: Kevin
 */ 


#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include "V2X_board.h"

static uint16_t state;

#define STATE_DEFAULT_VALUE (1<<ENABLE_3V3)|(0<<ENABLE_3V3B)|(0<<ENABLE_3V3C)|(0<<ENABLE_3V3D)|\
							(0<<ENABLE_4V1)|(0<<ENABLE_5V0)|(0<<ENABLE_5V0B)|(0<<ENABLE_SIM_WAKE)|\
							(0<<ENABLE_HUB)|(0<<ENABLE_CAN_SLEEP)|(0<<ENABLE_CAN_RESET)|(0<<ENABLE_SIM_PWR_ON)|\
							(0<<ENABLE_SIM_RESET)|(0<<ENABLE_SIM_RF_OFF)|(0<<ENABLE_SIM_VBUS)|(0<<ENABLE_FTDI_RESET)

typedef struct step {
	uint16_t value;    // output value
	uint16_t interval; // interval in ms
} step_t;

typedef struct power_state {
	uint16_t curr;	//current bit state
} power_state_t;

struct power_state power;
void shiftRegisterInit(void);
void shiftRegisterClear(void);
void shiftRegisterLatch(void);
void bits_To_Shift_Register(uint16_t bits);
void turn_on (uint16_t pin_name_on);
void turn_off(uint16_t pin_name_off);

#endif /* SEQUENCE_H_ */
/**
 * /file V2X_drivers.h
 *
 * /brief Hardware driver functions for operating the V2X board
 *
 * Author: Jesse Banks (jbanks2)
 **/

#ifndef V2X_DRIVERS_H_
#define V2X_DRIVERS_H_

#include "V2X_board.h"

/**
 * @def shift_register_state
 * @brief variable that holds the current power state.
 */
volatile uint16_t power_control_state;

/**
 * @def STATE_DEFAULT_VALUE
 * @brief default power state. enables 3v3 only
 */
#define POWER_CONTROL_DEFAULT_VALUE (1<<ENABLE_3V3)|(0<<ENABLE_3V3B)|(0<<ENABLE_3V3C)|(0<<ENABLE_3V3D)|\
									(0<<ENABLE_4V1)|(0<<ENABLE_5V0)|(0<<ENABLE_5V0B)|(0<<ENABLE_SIM_WAKE)|\
									(1<<ENABLE_HUB)|(0<<ENABLE_CAN_SLEEP)|(0<<ENABLE_CAN_RESET)|(0<<ENABLE_SIM_PWR_ON)|\
									(0<<ENABLE_SIM_RESET)|(0<<ENABLE_SIM_RF_OFF)|(0<<ENABLE_SIM_VBUS)|(0<<ENABLE_FTDI_RESET)

/**
 * @def shift_register_init
 * @brief	Resets the shift register, then sets to default values
 *			this turns on power supplies to maintain the atmel and other circuits
 *			not very specific yet, all function just turn on.
 **/
void PWR_init(void);

/**
 * @def shift_register_latch
 * @brief	causes the serial data just shifted into the shift register to latch to the outputs
 **/
void PWR_latch(void);

/**
 * @def shift_register_clear
 * @brief Causes the register to latch all outputs to low
 **/
void PWR_clear(void);

/**
 * @def state_to_shift_register
 * @brief	Sends the current power state to the shift register and latches to the outputs
 **/
void PWR_push(void);

/**
 * @def turn_on
 * @brief Sets bits in the power state variable, in preparation for a state change
 * @param pins_mask bit wise mask of enable/power pins
 **/
void PWR_turn_on(uint16_t pins_mask);

/**
 * @def shift_register_clear
 * @brief Clears bits in the power state variable, in preparation for a state change
 * @param pins_mask bit wise mask of enable/power pins
 **/
void PWR_turn_off(uint16_t pins_mask);

/**
 * @def PWR_query
 * @brief returns if the  
 * @param mask bitwise mask use with power_sequence_outputs
 * @retval if power_status_mask & var_mask != 0 returns true
 */
Bool PWR_query(uint16_t mask);

/**
 * @def power_hub_start
 * @brief sends a sequence to disable the USB hub
 */
void PWR_hub_start(void);

/**
 * @def power_hub_reset
 * @brief sends a sequence to disable the USB hub
 */
void PWR_hub_stop(void);

/**
 * @def PWR_is_5_needed
 * @brief turns off 5v rail if unused
 */
void PWR_is_5_needed (void);

/**
 * @def PWR_host_start
 * @brief sends power to the host computer
 */
void PWR_host_start(void);

/**
 * @def PWR_host_stop
 * @brief turns off power to host and 5v rail if unused
 */
void PWR_host_stop(void);

/**
 * @def PWR_can_stop
 * @brief disables the ELM with hardware reset pin turns off 5v rail if unused
*/
void PWR_can_stop (void);

/**
 * @def PWR_can_start
 * @brief enables the ELM with hardware reset pin
*/
void PWR_can_start (void);

/**
 * @def GSM_start
 * @brief sends a sequence to enable the SIM5320 module
 */
void PWR_gsm_start(void);

/**
 * @def power_sim_reset
 * @brief sends a sequence to disable the SIM5320 module
 */
void PWR_gsm_stop(void);

#endif /* V2X_DRIVERS_H_ */
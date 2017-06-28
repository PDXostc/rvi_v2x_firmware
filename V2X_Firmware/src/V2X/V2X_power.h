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
 * @def SHIFT_REGISTER_TYPE
 * @brief shift register type, bit length determined by hardware
 */
#if V2X_REV <= REV_12
#define SHIFT_REGISTER_TYPE uint16_t
#elif V2X_REV >= REV_20
#define SHIFT_REGISTER_TYPE uint8_t
#endif

/**
 * @def shift_register_state
 * @brief variable that holds the current power state.
 */
volatile SHIFT_REGISTER_TYPE power_control_state;

/**
 * @def STATE_DEFAULT_VALUE
 * @brief default power state. enables 3v3 only
 */
#if V2X_REV <= REV_12
#define POWER_CONTROL_DEFAULT_VALUE (1<<ENABLE_3V3)|        \
									(0<<ENABLE_3V3B)|       \
									(0<<ENABLE_3V3C)|       \
									(0<<ENABLE_3V3D)|       \
									(0<<ENABLE_4V1)|        \
									(0<<ENABLE_5V0)|        \
									(0<<ENABLE_5V0B)|       \
									(0<<ENABLE_SIM_WAKE)|   \
									(1<<ENABLE_HUB)|        \
									(0<<ENABLE_CAN_SLEEP)|  \
									(0<<ENABLE_CAN_RESET)|  \
									(0<<ENABLE_SIM_PWR_ON)| \
									(0<<ENABLE_SIM_RESET)|  \
									(0<<ENABLE_SIM_RF_OFF)| \
									(0<<ENABLE_SIM_VBUS)|   \
									(0<<ENABLE_FTDI_RESET)
#elif V2X_REV >= REV_20
#define POWER_CONTROL_DEFAULT_VALUE (1<<ENABLE_4V1)|        \
									(0<<ENABLE_5V0)|        \
									(0<<ENABLE_5V0B)|       \
									(0<<ENABLE_CAN_SLEEP)|  \
									(0<<ENABLE_CAN_RESET)|  \
									(0<<ENABLE_SIM_PWR_ON)| \
									(0<<ENABLE_SIM_RESET)|  \
									(0<<ENABLE_SIM_WAKE)
#endif
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
void PWR_turn_on(SHIFT_REGISTER_TYPE pins_mask);

/**
 * @def shift_register_clear
 * @brief Clears bits in the power state variable, in preparation for a state change
 * @param pins_mask bit wise mask of enable/power pins
 **/
void PWR_turn_off(SHIFT_REGISTER_TYPE pins_mask);

/**
 * @def PWR_query
 * @brief returns if the
 * @param mask bitwise mask use with power_sequence_outputs
 * @retval if power_status_mask & var_mask != 0 returns true
 */
Bool PWR_query(SHIFT_REGISTER_TYPE mask);

#if V2X_REV <= REV_12
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
#endif

#if V2X_REV >= REV_20
/**
 * @def PWR_3_start
 * @brief enable 3v pin
 */
void PWR_3_start(void);

/**
 * @def PWR_3_stop
 * @brief disable 3v pin
 */
void PWR_3_stop(void);

/**
 * @def PWR_3_is_needed
 * @brief check if 3v pin should be disabled
 */
void PWR_3_is_needed(void);

/**
 * @def PWR_4_start
 * @brief Enable 4v power supply, disabling 3v pin
 */
void PWR_4_start(void);

/**
 * @def PWR_4_stop
 * @brief Disable 4v power supply, after enabling 3v pin to keep CPU alive
 */
void PWR_4_stop(void);

/**
 * @def PWR_shutdown
 * @brief Disable all power. Manual restart of board required.
 */
void PWR_shutdown(void);

/**
 * @def PWR_5_start
 * @brief Start 5v rail. Also automatically activates 4v
 */
void PWR_5_start(void);
#endif

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
 * @def PWR_5_stop
 * @brief disables the 5v completely
*/
void PWR_5_stop(void);

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
 * @brief sends a sequence to enable the SIM module
 */
void PWR_gsm_start(void);

/**
 * @def PWR_gsm_stop
 * @brief sends a sequence to disable the SIM module
 */
void PWR_gsm_stop(void);

/**
 * @def power_sim_reset
 * @brief Forces reset of the SIM module. Please use sparingly, with caution.
 */
void PWR_gsm_reset(void);

#endif /* V2X_DRIVERS_H_ */
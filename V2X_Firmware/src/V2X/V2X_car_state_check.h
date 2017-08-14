/*
 * V2X_car_state_check.h
 *
 * Created: 8/9/2017 2:13:43 PM
 *  Author: dev-box
 */ 


#ifndef V2X_CAR_STATE_CHECK_H_
#define V2X_CAR_STATE_CHECK_H_


/**
 * @def CSC_CAR_STATES
 * @brief switch for selecting car off/on states
 */
typedef enum {
    CSC_car_state_unknown = 0,
    CSC_car_state_sleeping,
    CSC_car_state_running,
} CSC_CAR_STATE;


/**
 * @def CSC_SEQUENCE_STATES
 * @brief switch for selecting control states
 */
typedef enum {
    CSC_state_start = 0,
    CSC_state_low_power,
    CSC_state_high_power,
} CSC_SEQUENCE_STATE;

/**
 * @def CSC_LOW_POWER_SUBSEQUENCE_STATES
 * @brief generic switch for moving through control substates
 */
typedef enum {
    CSC_low_power_subsequence_1 = 0,
    CSC_low_power_subsequence_2,
    CSC_low_power_subsequence_3,
    CSC_low_power_subsequence_4,
    CSC_low_power_subsequence_5,
    CSC_low_power_subsequence_6,
    CSC_low_power_subsequence_7,
    CSC_low_power_subsequence_8,
    CSC_low_power_subsequence_9,
    CSC_low_power_subsequence_10,
    CSC_low_power_subsequence_COMPLETE,
    CSC_low_power_subsequence_FAIL
} CSC_LOW_POWER_SUBSEQUENCE_STATE;

/**
 * @def CSC_HIGH_POWER_SUBSEQUENCE_STATES
 * @brief generic switch for moving through control substates
 */
typedef enum {
    CSC_high_power_subsequence_1 = 0,
    CSC_high_power_subsequence_2,
    CSC_high_power_subsequence_3,
    CSC_high_power_subsequence_4,
    CSC_high_power_subsequence_5,
    CSC_high_power_subsequence_6,
    CSC_high_power_subsequence_7,
    CSC_high_power_subsequence_8,
    CSC_high_power_subsequence_9,
    CSC_high_power_subsequence_10,
    CSC_high_power_subsequence_COMPLETE,
    CSC_high_power_subsequence_FAIL
} CSC_HIGH_POWER_SUBSEQUENCE_STATE;


Bool CSC_disable_car_state_check();

Bool CSC_enable_car_state_check();


void CSC_car_state_check();




#endif /* V2X_CAR_STATE_CHECK_H_ */
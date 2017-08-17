/*
 * V2X_car_state_check.c
 *
 * Created: 8/9/2017 2:13:20 PM
 *  Author: Lilli Szafranski
 */

#include "V2X.h"


#define CSC_LOW_POWER_CAR_CHECK_DEFAULT_TIMEOUT  10
#define CSC_HIGH_POWER_CAR_CHECK_DEFAULT_TIMEOUT 3

#define CSC_CAN_INIT_TIMEOUT               1
#define CSC_CAN_INIT_RETRY_TIMEOUT         1
#define CSC_CAN_HEAR_CHATTER_TIMEOUT       1
#define CSC_CAN_HEAR_CHATTER_RETRY_TIMEOUT 1
#define CSC_CAN_READ_VOLTAGE_TIMEOUT       1
#define CSC_CAN_READ_VOLTAGE_RETRY_TIMEOUT 1

#define CSC_CAN_INIT_NUM_RETRIES         5
#define CSC_CAN_HEAR_CHATTER_NUM_RETRIES 5
#define CSC_CAN_READ_VOLTAGE_NUM_RETRIES 5

CSC_CAR_STATE                    CSC_car_state                    = CSC_car_state_unknown;
CSC_SEQUENCE_STATE               CSC_sequence_state               = CSC_state_start;
CSC_LOW_POWER_SUBSEQUENCE_STATE  CSC_low_power_subsequence_state  = CSC_low_power_subsequence_1;
CSC_HIGH_POWER_SUBSEQUENCE_STATE CSC_high_power_subsequence_state = CSC_high_power_subsequence_1;

uint16_t CSC_get_car_state_check_low_power_default_interval(void) {
    return CSC_LOW_POWER_CAR_CHECK_DEFAULT_TIMEOUT;
}

uint16_t CSC_get_car_state_check_high_power_default_interval(void) {
    return CSC_HIGH_POWER_CAR_CHECK_DEFAULT_TIMEOUT;
}

uint8_t CSC_get_car_state_check_default_enabled(void) {
    return CSC_CAR_STATE_CHECK_ENABLED;
}

uint16_t CSC_low_power_car_check_timeout(void);

uint16_t CSC_low_power_car_check_timeout(void) {
    return eeprom_read_unsigned_int_16(EE_car_state_check_low_power_check_interval);
}

uint16_t CSC_high_power_car_check_timeout(void);

uint16_t CSC_high_power_car_check_timeout(void) {
    return eeprom_read_unsigned_int_16(EE_car_state_check_high_power_check_interval);
}

uint16_t CSC_get_timeout_for_car_state(void);

uint16_t CSC_get_timeout_for_car_state(void) {
    if (CSC_car_state == CSC_car_state_running)
        return CSC_high_power_car_check_timeout();

    return CSC_low_power_car_check_timeout();
}

void CSC_enable_car_state_check(void) {
    if (CSC_sequence_state != CSC_state_start) /* Then we are in the middle of a thing and really shouldn't overwrite the check timeout; assume already be enabled. */
        return;

    nvm_eeprom_write_byte(EE_car_state_check_enabled, CSC_CAR_STATE_CHECK_ENABLED);

    job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());
}

void CSC_disable_car_state_check(void) {
    if (CSC_sequence_state == CSC_state_low_power) {
        CSC_low_power_subsequence_state = CSC_low_power_subsequence_FAIL;
        CSC_car_state_check();

    } else if (CSC_sequence_state == CSC_state_high_power) {
        CSC_high_power_subsequence_state = CSC_high_power_subsequence_FAIL;
        CSC_car_state_check();

    }

    nvm_eeprom_write_byte(EE_car_state_check_enabled, CSC_CAR_STATE_CHECK_DISABLED);

    job_clear_timeout(SYS_CAR_STATE_CHECK);
}

void CSC_car_state_low_power_flow(void);
void CSC_car_state_high_power_flow(void);

void CSC_handle_low_power_sequence_fail(const char *message);
void CSC_handle_high_power_sequence_fail(const char *message);

void CSC_print_to_CMD(const char* message);

void CSC_print_to_CMD(const char* message) {
		menu_send_CSC();
		usb_tx_string_PVO(message);
		menu_send_n_st();
}

void CSC_handle_low_power_sequence_fail(const char *message) {
	CSC_low_power_subsequence_state = CSC_low_power_subsequence_FAIL;
	CSC_car_state_check();

	CSC_print_to_CMD(message);
}

void CSC_handle_high_power_sequence_fail(const char *message) {
	CSC_high_power_subsequence_state = CSC_high_power_subsequence_FAIL;
	CSC_car_state_check();

	CSC_print_to_CMD(message);
}

/*
    Updated functionality...

    In summary:
    First check if low power. If not low power, check if car is still running by listening to the CAN bus;
    shut down the raspi if we don't hear chatter on the CAN bus (i.e, car is not running). Reschedule the
    job.

    If low power, do the CAN initialization stuff so that we can listen to the CAN bus and check the voltage. (This
    will pull us out of low power, so if the checks put us back into the 'resume-sleeping' state, we need
    to make sure to go back to low power.) Check if the car is running by listening to the CAN bus; start raspi if
    we heard chatter and reschedule the job. If not, check the voltage to make sure the battery isn't almost dead. If it is, shut everything
    down, otherwise reschedule the job.

    In detail:
    Check if low power or high power

    If low power:
        {//Change power state to power STN chip
            PWR_4_start(); starts 4V and stops low power 3V, as 4V now provides 3V
            CAN_elm_init(); //starts 5V, STN enable signals
        }

        Configure can device ();

        Listen for can activity ()

        IF CanBusActive
                -- PWR_mode_high(); //start raspi-bring-up sequence
                -- reschedule job (at shorter interval)
        ELSE
                --  check the battery voltage
        IF (Battery < 11V)
                -- PWR_shutdown();
        ELSE  kill the 5&4V,
                -- PWR_mode_low();
                -- reschedule job (at longer interval)

    If high power:
        IF CanBusActive
                -- reschedule job (at shorter interval)
        ELSE
                -- PWR_mode_low(); //kills everything but low power 3V
                -- reschedule job (at longer interval)

*/
void CSC_car_state_check(void) {

    switch (CSC_sequence_state) {
        case CSC_state_start:
            menu_send_CSC();
            usb_tx_string_PVO(PSTR("Car-state check - "));

            if (PWR_is_low_power()) {

                usb_tx_string_PVO(PSTR("low power!\r\n>"));

                CSC_sequence_state = CSC_state_low_power;
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_1;
            } else {
                usb_tx_string_PVO(PSTR("high power!\r\n>"));

                CSC_sequence_state = CSC_state_high_power;
                CSC_high_power_subsequence_state = CSC_high_power_subsequence_1;
            }

            CSC_car_state_check();

            break;

        case CSC_state_low_power:
            CSC_car_state_low_power_flow();
            break;

        case CSC_state_high_power:
            CSC_car_state_high_power_flow();
            break;
    }
}

void CSC_car_state_low_power_flow(void) {
    static uint8_t retries = 0;

    switch (CSC_low_power_subsequence_state) {
        case CSC_low_power_subsequence_1:
            if (CAN_get_sequence_state() != CAN_state_idle) { /* If for some reason the CAN is in the middle of something else, just fail and check later */
                CSC_handle_low_power_sequence_fail(PSTR("Car-state check failed - CAN isn't idle"));

            } else { /* Otherwise, power-on CAN and start our check sequence */
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_2;

                CSC_print_to_CMD(PSTR("Car-state check - starting CAN"));

                PWR_4_start();
                CAN_elm_init();
            
			    retries = 0;
                job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_INIT_TIMEOUT);
            }

            break;

        case CSC_low_power_subsequence_2: /* Is CAN successfully online? */

            if (CAN_get_init_subsequence_state() == CAN_init_subsequence_COMPLETE) { /* If it completed, we move on */
	            CSC_low_power_subsequence_state = CSC_low_power_subsequence_3;
	            CSC_car_state_check();
				
			} else if (CAN_get_init_subsequence_state() == CAN_init_subsequence_FAIL) { /* If it failed, we fail, so reschedule for later */
                CSC_handle_low_power_sequence_fail(PSTR("Car-state check failed - CAN initialization failed"));

            } else { /* Otherwise, try again soon */
                if (retries > CSC_CAN_INIT_NUM_RETRIES) {
                    CSC_handle_low_power_sequence_fail(PSTR("Car-state check failed - CAN initialization timeout"));

                } else {
                    retries++;
                    job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_INIT_RETRY_TIMEOUT);

                    CSC_print_to_CMD(PSTR("Retry"));
                }
            }

            break;

        case CSC_low_power_subsequence_3: /* CAN is online, so start the process for listening to CAN chatter */
            CSC_low_power_subsequence_state = CSC_low_power_subsequence_4;

            CSC_print_to_CMD(PSTR("Car-state check - listening for CAN chatter"));

            CAN_hear_chatter_start();

			retries = 0;
			job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_HEAR_CHATTER_TIMEOUT);

            break;

        case CSC_low_power_subsequence_4: /* Do we have CAN chatter? */

            if (CAN_get_hear_chatter_subsequence_state() == CAN_hear_chatter_subsequence_COMPLETE) {
                if (CAN_get_last_did_hear_chatter()) { /* If we do, turn on the raspi, and reset our job */

                    CSC_print_to_CMD(PSTR("Car-state check - CAN chatter heard; powering up"));

                    CSC_sequence_state = CSC_state_start;
                    CSC_low_power_subsequence_state = CSC_low_power_subsequence_COMPLETE;
                    CSC_car_state = CSC_car_state_running;

                    PWR_set_wake_up_reason(PWR_WAKE_UP_REASON_CAR_RUNNING);

                    PWR_mode_high();

                    job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());

                } else { /* If we don't, check that the battery voltage isn't too low */
                    CSC_low_power_subsequence_state = CSC_low_power_subsequence_5;
					CSC_car_state_check();
					
                }
            } else if (CAN_get_hear_chatter_subsequence_state() == CAN_hear_chatter_subsequence_FAIL) {
                CSC_handle_low_power_sequence_fail(PSTR("Car-state check failed - listening for CAN chatter failed"));
				
            } else { /* Otherwise, try again soon */
				if (retries > CSC_CAN_HEAR_CHATTER_NUM_RETRIES) {
					CSC_handle_low_power_sequence_fail(PSTR("Car-state check failed - CAN hear chatter timeout"));

				} else {
					retries++;
					job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_HEAR_CHATTER_RETRY_TIMEOUT);

					CSC_print_to_CMD(PSTR("Retry"));
				}
            }

            break;

        case CSC_low_power_subsequence_5: /* What is the battery voltage? */
            CSC_low_power_subsequence_state = CSC_low_power_subsequence_6;
            
			CSC_print_to_CMD(PSTR("Car-state check - CAN chatter not heard; checking voltage"));

			CAN_read_voltage_start();

			retries = 0;
			job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_READ_VOLTAGE_TIMEOUT);
			
			break;
			
        case CSC_low_power_subsequence_6: /* What is the battery voltage? */

            if (CAN_get_read_voltage_subsequence_state() == CAN_read_voltage_subsequence_COMPLETE) {
                if (CAN_get_last_read_voltage() < 11.0) { /* The voltage is too low, shut everything down */

                    CSC_print_to_CMD(PSTR("Car-state check - voltage low; powering off"));

                    PWR_shutdown();

                } else { /* Reset our car-state check */
                    CSC_low_power_subsequence_state = CSC_low_power_subsequence_COMPLETE;

                    CSC_sequence_state = CSC_state_start;
                    CSC_car_state = CSC_car_state_sleeping;

                    CSC_print_to_CMD(PSTR("Car-state check - rescheduling job"));

                    PWR_mode_low();

                    job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());
                }

            } else if (CAN_get_read_voltage_subsequence_state() == CAN_read_voltage_subsequence_FAIL) {
                CSC_handle_low_power_sequence_fail(PSTR("Car-state check failed - reading voltage failed"));
				
            } else { /* Otherwise, try again soon */
				if (retries > CSC_CAN_READ_VOLTAGE_NUM_RETRIES) {
					CSC_handle_low_power_sequence_fail(PSTR("Car-state check failed - CAN read voltage timeout"));

				} else {
					retries++;
					job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_READ_VOLTAGE_RETRY_TIMEOUT);

					CSC_print_to_CMD(PSTR("Retry"));
				}
			}

            break;

        default:
        case CSC_low_power_subsequence_FAIL:
            CSC_sequence_state = CSC_state_start;
            CSC_car_state = CSC_car_state_sleeping;

            PWR_mode_low();

            job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());

            break;
    }
}

void CSC_car_state_high_power_flow(void) {
    static uint8_t retries = 0;

    switch (CSC_high_power_subsequence_state) {
        case CSC_high_power_subsequence_1:
            if (CAN_get_sequence_state() != CAN_state_idle) { /* If for some reason the CAN is starting up by someone else or in some other state, just fail and check later */
                CSC_handle_high_power_sequence_fail(PSTR("Car-state check failed - CAN isn't idle"));

            } else { /* Otherwise, start our check sequence */
                CSC_high_power_subsequence_state = CSC_high_power_subsequence_2;
                CSC_car_state_check();
				
            }

            break;

        case CSC_high_power_subsequence_2: /* CAN is online, so start the process for listening to CAN chatter */
            CSC_high_power_subsequence_state = CSC_high_power_subsequence_3;

            CSC_print_to_CMD(PSTR("Car-state check - listening for CAN chatter"));

            CAN_hear_chatter_start();

			retries = 0;
			job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_HEAR_CHATTER_TIMEOUT);

            break;

        case CSC_high_power_subsequence_3: /* Do we have CAN chatter? */

            if (CAN_get_hear_chatter_subsequence_state() == CAN_hear_chatter_subsequence_COMPLETE) {
                if (CAN_get_last_did_hear_chatter()) { /* If we do, just reset our job */
                    CSC_car_state = CSC_car_state_running;

                    CSC_print_to_CMD(PSTR("Car-state check - CAN chatter heard; rescheduling job"));

                } else { /* If we don't, put everything to sleep */
                    CSC_car_state = CSC_car_state_sleeping;

                    CSC_print_to_CMD(PSTR("Car-state check - CAN chatter not heard; powering down"));

                    PWR_mode_low();
                }

                CSC_sequence_state = CSC_state_start;
                CSC_high_power_subsequence_state = CSC_high_power_subsequence_COMPLETE;

                job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());

            } else if (CAN_get_hear_chatter_subsequence_state() == CAN_hear_chatter_subsequence_FAIL) {
                CSC_handle_high_power_sequence_fail(PSTR("Car-state check failed - listening for CAN chatter failed"));
				
            } else {
				if (retries > CSC_CAN_HEAR_CHATTER_NUM_RETRIES) {
					CSC_handle_high_power_sequence_fail(PSTR("Car-state check failed - CAN hear chatter timeout"));

				} else {
					retries++;
					job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_HEAR_CHATTER_RETRY_TIMEOUT);

					CSC_print_to_CMD(PSTR("Retry"));
				}
			}

            break;

        default:
        case CSC_high_power_subsequence_FAIL:
            CSC_sequence_state = CSC_state_start;
            CSC_car_state = CSC_car_state_running;

            job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());
    }
}


/*
 * V2X_car_state_check.c
 *
 * Created: 8/9/2017 2:13:20 PM
 *  Author: Lilli Szafranski
 */

#include "V2X.h"


#define CSC_LOW_POWER_CAR_CHECK_DEFAULT_TIMEOUT  10
#define CSC_HIGH_POWER_CAR_CHECK_DEFAULT_TIMEOUT 3

#define CSC_CAN_START_TIMEOUT                 12
#define CSC_CAN_START_RECHECK_TIMEOUT         1
#define CSC_CAN_CHATTER_TIMEOUT               15
#define CSC_CAN_CHECK_BATTERY_VOLTAGE_TIMEOUT 4

CSC_CAR_STATE                    CSC_car_state         = CSC_car_state_unknown;
CSC_SEQUENCE_STATE               CSC_sequence_state    = CSC_state_start;
CSC_LOW_POWER_SUBSEQUENCE_STATE  CSC_low_power_subsequence_state  = CSC_low_power_subsequence_1;
CSC_HIGH_POWER_SUBSEQUENCE_STATE CSC_high_power_subsequence_state = CSC_high_power_subsequence_1;

uint8_t CSC_low_power_car_check_timeout() {
    // TODO: Read from eeprom
    return CSC_LOW_POWER_CAR_CHECK_DEFAULT_TIMEOUT;
}

uint8_t CSC_high_power_car_check_timeout() {
    // TODO: Read from eeprom
    return CSC_HIGH_POWER_CAR_CHECK_DEFAULT_TIMEOUT;
}

uint8_t CSC_get_timeout_for_car_state() {
    if (CSC_car_state == CSC_car_state_running)
        return CSC_high_power_car_check_timeout();

    return CSC_low_power_car_check_timeout();
}

Bool CSC_enable_car_state_check() {
    if (CSC_sequence_state != CSC_state_start) /* Then we are in the middle of a thing and really shouldn't enable the check; it'll overwrite our timer job. */
        return false;

    if (CSC_car_state == CSC_car_state_transitioning_up || CSC_car_state == CSC_car_state_transitioning_down)
        return false;

    job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());

    return true;
}

Bool CSC_disable_car_state_check() {
    if (CSC_sequence_state != CSC_state_start) /* Then we are in the middle of a thing and really shouldn't disable the check; it'll overwrite our timer job. */
        return false;

    if (CSC_car_state == CSC_car_state_transitioning_up || CSC_car_state == CSC_car_state_transitioning_down)
        return false;

    job_clear_timeout(SYS_CAR_STATE_CHECK);

    return true;
}

void CSC_car_state_low_power_flow();
void CSC_car_state_high_power_flow();

/*
    Updated functionality...

    In summary:
    First check if low power. If not low power, check if car is still running by listening to the CAN bus;
    shut down the raspi if it is not running.

    If low power, do the CAN initialization stuff. This will pull us out of low power, so if the check puts us back
    into the 'do nothing' state, we need to make sure to go back to low power so that our isLowPower check is reset.

    In detail:
    Check if low power or high power

    If low power:
        {//Change power state to power STN chip
            PWR_4_start(); starts 4V and stops low power 3V, as 4V now provides 3V
            PWR_can_start(); //starts 5V, STN enable signals
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
                --  PWR_4_stop(); //stops 4V and starts low power 3V
                -- PWR_5_stop(); //stops 5V, drops STN enable signals
                -- reschedule job (at longer interval)

    If high power:
        IF CanBusActive
                --  reschedule job (at shorter interval)
        ELSE
                -- PWR_mode_low (); //kills everything but low power 3V
                -- reschedule job (at longer interval)

*/
void CSC_car_state_check() {

    switch (CSC_sequence_state) {
        case CSC_state_start:
            menu_send_CSC();
            usb_tx_string_PVO(PSTR("Car-state check - "));

            if (PWR_is_low_power()) {

                usb_tx_string_PVO(PSTR("low power!\r\n"));

                CSC_sequence_state = CSC_state_low_power;
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_1;
            } else {
                usb_tx_string_PVO(PSTR("high power!\r\n"));

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

void CSC_car_state_low_power_flow() {
    switch (CSC_low_power_subsequence_state) {
        case CSC_low_power_subsequence_1:
            if (CAN_get_sequence_state() != CAN_state_idle) { /* If for some reason the CAN is starting up by someone else, just fail and check later */
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_FAIL;
                CSC_car_state_check();

                menu_send_CSC();
                usb_tx_string_PVO(PSTR("Car-state check failed - CAN isn't idle\n\r"));

            } else { /* Otherwise, power-on CAN and start our check sequence */
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_2;

                menu_send_CSC();
                usb_tx_string_PVO(PSTR("Car-state check - starting CAN\n\r"));

                PWR_4_start();
                CAN_elm_init();

                job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_START_TIMEOUT);
            }

            break;

        case CSC_low_power_subsequence_2: /* Is CAN successfully online? */

            if (CAN_get_init_subsequence_state() == CAN_init_subsequence_FAIL) { /* If it failed, we fail, and try again later */
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_FAIL;
                CSC_car_state_check();

                menu_send_CSC();
                usb_tx_string_PVO(PSTR("Car-state check failed - CAN initialization failed\n\r"));

            } else if (CAN_get_init_subsequence_state() == CAN_init_subsequence_COMPLETE) { /* If it completed, we move on */
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_3;
                CSC_car_state_check();

            } else { /* Otherwise, try again soon */
                // TODO: Only try so many times
                job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_START_RECHECK_TIMEOUT);

            }

            break;

        case CSC_low_power_subsequence_3: /* CAN is online, so start the process for listening to CAN chatter */
            CSC_low_power_subsequence_state = CSC_low_power_subsequence_4;

            menu_send_CSC();
            usb_tx_string_PVO(PSTR("Car-state check - listening for CAN chatter\n\r"));

            CAN_hear_chatter_start();

            job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_CHATTER_TIMEOUT);

            break;

        case CSC_low_power_subsequence_4: /* Do we have CAN chatter? */

            if (CAN_get_hear_chatter_subsequence_state() == CAN_hear_chatter_subsequence_COMPLETE) {
                if (CAN_get_last_did_hear_chatter()) { /* If we do, turn on the raspi, and reset our job */

                    menu_send_CSC();
                    usb_tx_string_PVO(PSTR("Car-state check - CAN chatter heard; powering up\n\r"));

                    CSC_sequence_state = CSC_state_start;
                    CSC_low_power_subsequence_state = CSC_low_power_subsequence_COMPLETE;
                    CSC_car_state = CSC_car_state_running;

                    PWR_mode_high();

                    job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());

                } else { /* If we don't, check that the battery voltage isn't too low */
                    CSC_low_power_subsequence_state = CSC_low_power_subsequence_5;

                    menu_send_CSC();
                    usb_tx_string_PVO(PSTR("Car-state check - CAN chatter not heard; checking voltage\n\r"));

                    CAN_read_voltage_start();

                    job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_CHECK_BATTERY_VOLTAGE_TIMEOUT);

                }
            } else {
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_FAIL;
                CSC_car_state_check();

                menu_send_CSC();
                usb_tx_string_PVO(PSTR("Car-state check failed - listening for CAN chatter failed\n\r"));
            }

            break;

        case CSC_low_power_subsequence_5: /* What is the battery voltage? */
            // TODO: Check voltage value

            if (CAN_get_read_voltage_subsequence_state() == CAN_read_voltage_subsequence_COMPLETE) {
                if (CAN_get_last_read_voltage() < 11.0) { /* The voltage is too low, shut everything down */

                    menu_send_CSC();
                    usb_tx_string_PVO(PSTR("Car-state check - voltage low; powering off\n\r"));

                    PWR_shutdown();

                } else { /* Reset our car-state check */
                    CSC_low_power_subsequence_state = CSC_low_power_subsequence_COMPLETE;

                    CSC_sequence_state = CSC_state_start;
                    CSC_car_state = CSC_car_state_sleeping;

                    menu_send_CSC();
                    usb_tx_string_PVO(PSTR("Car-state check - rescheduling job\n\r"));

                    PWR_mode_low();

                    job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());
                }

            } else {
                CSC_low_power_subsequence_state = CSC_low_power_subsequence_FAIL;
                CSC_car_state_check();

                menu_send_CSC();
                usb_tx_string_PVO(PSTR("Car-state check failed - reading voltage failed\n\r"));
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

void CSC_car_state_high_power_flow() {

    switch (CSC_high_power_subsequence_state) {
        case CSC_high_power_subsequence_1:
            if (CAN_get_sequence_state() != CAN_state_idle) { /* If for some reason the CAN is starting up by someone else or in some other state, just fail and check later */
                CSC_high_power_subsequence_state = CSC_high_power_subsequence_FAIL;
                CSC_car_state_check();

                menu_send_CSC();
                usb_tx_string_PVO(PSTR("Car-state check failed - CAN isn't idle\n\r"));

            } else { /* Otherwise, start our check sequence */
                CSC_high_power_subsequence_state = CSC_high_power_subsequence_2;
                CSC_car_state_check();
            }

            break;

        case CSC_high_power_subsequence_2: /* CAN is online, so start the process for listening to CAN chatter */
            CSC_high_power_subsequence_state = CSC_high_power_subsequence_3;

            menu_send_CSC();
            usb_tx_string_PVO(PSTR("Car-state check - listening for CAN chatter\n\r"));

            CAN_hear_chatter_start();

            job_set_timeout(SYS_CAR_STATE_CHECK, CSC_CAN_CHATTER_TIMEOUT);

            break;

        case CSC_high_power_subsequence_3: /* Do we have CAN chatter? */

            if (CAN_get_hear_chatter_subsequence_state() == CAN_hear_chatter_subsequence_COMPLETE) {
                if (CAN_get_last_did_hear_chatter()) { /* If we do, just reset our job */
                    CSC_car_state = CSC_car_state_running;

                    menu_send_CSC();
                    usb_tx_string_PVO(PSTR("Car-state check - CAN chatter heard; rescheduling job\n\r"));

                } else { /* If we don't, put everything to sleep */
                    CSC_car_state = CSC_car_state_sleeping;

                    menu_send_CSC();
                    usb_tx_string_PVO(PSTR("Car-state check - CAN chatter not heard; powering down\n\r"));

                    PWR_mode_low();
                }

                CSC_sequence_state = CSC_state_start;
                CSC_high_power_subsequence_state = CSC_high_power_subsequence_COMPLETE;

                job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());

            } else {
                CSC_high_power_subsequence_state = CSC_high_power_subsequence_FAIL;
                CSC_car_state_check();

                menu_send_CSC();
                usb_tx_string_PVO(PSTR("Car-state check failed - listening for CAN chatter failed\n\r"));
            }

            break;

        default:
        case CSC_high_power_subsequence_FAIL:
            CSC_sequence_state = CSC_state_start;
            CSC_car_state = CSC_car_state_running;

            job_set_timeout(SYS_CAR_STATE_CHECK, CSC_get_timeout_for_car_state());
    }
}


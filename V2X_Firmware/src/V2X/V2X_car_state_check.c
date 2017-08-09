/*
 * V2X_car_state_check.c
 *
 * Created: 8/9/2017 2:13:20 PM
 *  Author: Lilli Szafranski
 */

#include "V2X.h"

void PWR_car_on_state_check() {


    usb_tx_string_PV(PSTR("Car-on sleep-state check!\n"));

    if (PWR_is_low_power()) {



        job_set_timeout(SYS_CAR_ON_STATE_CHECK, 3);

    } else {



        job_set_timeout(SYS_CAR_ON_STATE_CHECK, 3);

    }

    /*
     *
     * first check if low power. if not low power, check if car is still running by listening to the CAN bus.
     *
     * if low power, do the can stuff. this will pull us out of low power, so if the check puts us back into the 'do
     * nothing' state, we need to make sure to go back to low power so that our isLowPower check is reset.
     */


    /*
        This function must:
        Check if low power or high power

        If low power:
            Change power state to enable CAN device (because we're in low power mode)
                (turn on the 4 and the 5 and it will give me the 3, maybe? - probably happens automatically)

            Configure can device
                ()

            Take measurement of battery voltage (to see if we need to kill ourselves completely?)
                ()

            Decide what to do:
                - less than 11V do no reschedule checking job, power off
                - less than 13V and more than 11V reschedule checking job
                - more than 13V needs follow up
                    -- configure can setup (wouldn't this already be done?)
                    -- check for RPM (scratch that... just listen for CAN noise)
                    -- start raspi bring up sequence if greater than 0 (keep the job, though)

        If high power:
            Listen for CAN noise
                - If silent, kill the raspi, reschedule job (at longer interval)
                - If not silent, reschedule job (at shorter interval)
    */
}

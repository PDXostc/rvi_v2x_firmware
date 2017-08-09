/*
 * V2X_car_state_check.c
 *
 * Created: 8/9/2017 2:13:20 PM
 *  Author: Lilli Szafranski
 */

#include "V2X.h"

void CSC_car_state_check_init() {


    usb_tx_string_PV(PSTR("Car-on sleep-state check!\n"));

    if (PWR_is_low_power()) {



        job_set_timeout(SYS_CAR_ON_STATE_CHECK, 3);

    } else {



        job_set_timeout(SYS_CAR_ON_STATE_CHECK, 3);

    }

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
}

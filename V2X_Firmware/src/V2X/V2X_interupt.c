/*
 * V2X_interupt.c
 *
 * Created: 9/17/2017 10:21:34 PM
 *  Author: Jesse Banks
 */ 

#include "V2X.h"

/* Interrupt service routine for our button, on currently found on PORTA0
 * Because this pin is shared with other interesting pins, namely 3v, we
 * have to be careful about what we do with the interrupt.
 * Before proceeding, we check the state of the button pin, ensuring we are
 * reacting to button press. If so, we want to disable the 3v as quickly as
 * possible, so that the 4v and 3v are not fighting.
 * This routine will sense the state of the button, and will not exit until
 * release. Upon release, it reports the time held, and schedules a job to
 * react to the button press in (1) second.
 * If 3v was enabled when we entered it is enabled again before exit, so the
 * Atmel will stay alive, but 3v is checked against the 4v enable a final time
 * to ensure proper power state when leaving this routine.
 */
ISR(SW0_INT_VECT_0)
{
	if (ioport_get_pin_level(SW0_PIN) == SW0_ACTIVE)
	{
		// ask if 3v is up when the button was pressed
		bool en_3v = ioport_get_pin_level(PWR_3V3_PIN);

		//disable 3v while button down, so 3 and 4 don't fight
		PWR_3_stop();

		/* Hold in interrupt while pin tests held
		 *		service timer and increment time held
		 * When released, exit the interrupt routine and report time held
		 *		(call the button push consequence routine)
		 */
		while (ioport_get_pin_level(SW0_PIN) == SW0_ACTIVE)
		{
			//button_service(); //call service and record time
		}

		//button_service(); //call service and final delta

		// handle_button_check(button_get_delta());
		//job_set_timeout(SYS_PWR, 1);

		// if 3v was up when we entered, and 4v is NOT enabled, turn it right back on
		if (en_3v == true);
		{
			PWR_3_start();
		}
		// double check if 3 should be enabled before leaving, just in case
		PWR_3_is_needed();
	}
	
	USB_vbus_mount();
	
}


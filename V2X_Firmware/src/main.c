/**
 * \file Main.c
 *
 * \brief Initial firmware for V2X bring up
 *
 * Author: Jesse Banks (jbanks2)
 */

/**
 * \mainpage V2X Application documentation
 *
 *
 */

#include <asf.h>
#include "V2X/V2X.h"

#ifndef V2X_CLOCK_DEF
#warning Clock file is not updated. please follow instructions in src/V2X/conf_clock.h header.
#endif

#ifndef V2X_USB_DEF
#warning USB file is not updated. please follow instructions in src/V2X/conf_usb.h header.
#endif

#ifndef V2X_BOARD_DEF
#warning Board file is not updated. please follow instructions in src/V2X/conf_board.h header.
#endif

int main ()
{
	v2x_board_init();	//configure pins and initial safe condition

	while (1){
		//sleepmgr_enter_sleep();		//go to sleep until interrupt
		#if V2X_REV <= REV_12
			charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
		#endif
		//FIXME: bug here of some kind, sleep/or wd might be resetting proc
		//reset_processor();			//look for pending resets
		// FIXME: BUG HERE  not allowing progress past button check
		//button_service();			//SCAN and report the button
		#if V2X_REV <= REV_12
			charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
		#endif
		job_coordinator();			//schedule new jobs if needed
		GSM_process_buffer();		//handle any pending jobs for GSM
		CAN_process_buffer();		//handle any pending jobs for CAN
		#if V2X_REV <= REV_12
			charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
		#endif
		if (usb_cdc_is_active(USB_ACL)) //if host listening,
			{report_accel_data();}   //create and send accel data
		#if V2X_REV <= REV_12
			charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
		#endif
		//use leds for debugging
		led_update();
	}
}

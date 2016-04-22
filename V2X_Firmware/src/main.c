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
#warning Clock file is not updated. please follow instructions in src/V2X/clock_conf.h header.
#endif

int main ()
{
	v2x_board_init();	//configure pins and initial safe condition

	while (1){
		sleepmgr_enter_sleep();		//go to sleep until interrupt
			charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
		reset_processor();			//look for pending resets
		button_service();			//SCAN and report the button 
			charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
		job_coordinator();			//schedule new jobs if needed
		GSM_process_buffer();		//handle any pending jobs for GSM
		CAN_process_buffer();		//handle any pending jobs for CAN
			charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
		if (usb_cdc_is_active(USB_ACL)) //if host listening,
			{report_accel_data();}   //create and send accel data
			charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
	}
}

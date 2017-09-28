/**
 * \file main.c
 *
 * \mainpage V2X Application documentation
 *  Author: Jesse Banks
 *
 */

#include <asf.h>
#include "V2X/V2X.h"

#ifndef V2X_CLOCK_DEF
#warning conf_clock.h file is not updated. please follow instructions in src/V2X/OtherFiles/readme.md.
#endif

#ifndef V2X_USB_DEF
#warning conf_usb.h file is not updated. please follow instructions in src/V2X/OtherFiles/readme.md.
#endif

#ifndef V2X_CONF_BOARD_DEF
#warning conf_board.h file is not updated. please follow instructions in src/V2X/OtherFiles/readme.md.
#endif

#ifndef V2X_UART_DEF
#warning conf_usart_serial.h file is not updated. please follow instructions in src/V2X/OtherFiles/readme.md.
#endif

#ifndef V2X_OSC_DEF
#warning user_board.h file is not updated. please follow instructions in src/V2X/OtherFiles/readme.md.
#endif

int main (void)
{
	board_init();
	
	while (1) {
// 				sleepmgr_enter_sleep();		//go to sleep until interrupt
 				reset_processor();			//look for pending resets
 				job_coordinator();			//schedule new jobs if needed
 				GSM_process_buffer();		//handle any pending jobs for GSM
				CAN_process_buffer();		//handle any pending jobs for CAN
 				if (USB_port_is_active(USB_ACL)) //if host listening,
	  				{report_accel_data();}   //create and send accel data
				led_update();
	}
}

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
		//copies status signals from around the board to the LEDs, purely test code
		if (simcard_status() == true)							{led_0_on();	}
		else													{led_0_off();	}
		if (ioport_get_pin_level(EXT1_PIN_SIM_NETWORK) == true)	{led_1_on();	}
		else													{led_1_off();	}
		if (ioport_get_pin_level(EXT1_PIN_SIM_PWR) == true)		{led_2_on();	}
		else													{led_2_off();	}
		
 		charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
	}
}

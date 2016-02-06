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
#include "V2X/V2X_init.h"

#ifndef V2X_CLOCK_DEF
you made a mistake.
#warning Clock file is not updated. please follow instructions in src/V2X/clock_conf.h header.
#endif

int main ()
{
	sysclk_init();		//configure clock sources for core and USB
	v2x_board_init();	//configure pins and initial safe condition
	delay_ms(200);		//allow power to stabilize
	
	turn_on((1<<ENABLE_4V1)|(1<<ENABLE_SIM_RESET)|(1<<ENABLE_5V0)); // turn on GSM device power, allow out of reset
	state_to_shift_register();  //update shift register state
	delay_ms(200);				//allow power to stabilize
	
	turn_on((1<<ENABLE_SIM_PWR_ON)|(1<<ENABLE_SIM_RF_OFF)|(1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP)); //beginning of power on pulse
	state_to_shift_register();  //update shift register state
	delay_ms(100);				//allow power to stabilize
	
	turn_off((1<<ENABLE_SIM_PWR_ON)); //end of "on power" on pulse to gsm device
	state_to_shift_register();  //update shift register state
	
	while (1){
		//copies status signals from around the board to the LEDs
		if (simcard_status() == true)							{led_0_on();	}
		else													{led_0_off();	}
		if (ioport_get_pin_level(EXT1_PIN_SIM_NETWORK) == true)	{led_1_on();	}
		else													{led_1_off();	}
		if (ioport_get_pin_level(EXT1_PIN_SIM_PWR) == true)		{led_2_on();	}
		else													{led_2_off();	}
		
		charge_pump_toggle();		//charge pump pin needs toggled to create boost voltage for LEDs
	}
	
}

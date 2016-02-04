/**
 * \file
 *
 * \Initial firmware for V2X bring up
 *
 */

/**
 * \mainpage V2X Application documentation
 *
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
//#include "V2X/V2X_sequence.h"
//#include "conf_board.h"
//#include "V2X/V2X_drivers.h"
#include "V2X/V2X_init.h"

int main ()
{
	sysclk_init();
	v2x_board_init();	
	delay_ms(200);
	turn_on((1<<ENABLE_4V1)|(1<<ENABLE_SIM_RESET)|(1<<ENABLE_5V0)); // turn on GSM device power, allow out of reset
	delay_ms(200);
	gpio_set_pin_high(BUF1_PIN);
	turn_on((1<<ENABLE_SIM_PWR_ON)|(1<<ENABLE_SIM_RF_OFF)|(1<<ENABLE_CAN_RESET)|(1<<ENABLE_CAN_SLEEP)); //beggining of power on pulse
	delay_ms(100);
	turn_off((1<<ENABLE_SIM_PWR_ON)); //end of power on pulse
	while (1){
		
		if (simcardStatus() == true)							{led_0_on();	}
		else													{led_0_off();	}
		//toggleChargePump();	
		chargePumpToggle();								
		if (ioport_get_pin_level(EXT1_PIN_SIM_NETWORK) == true)	{led_1_on();	}
		else													{led_1_off();	}
		if (ioport_get_pin_level(EXT1_PIN_SIM_PWR) == true)		{led_2_on();	}
		else													{led_2_off();	}
		//toggleChargePump();
		chargePumpToggle();
// 		chargePumpToggle();
// 		chargePumpToggle();
// 		chargePumpToggle();
// 		chargePumpToggle();
// 		chargePumpToggle();
// 		
// 		canbusSerialRouting(BUFFER_AVR_ROUTING);
// 		delay_ms(1000);
// 		led_0_on();
// 		canbusSerialRouting(BUFFER_FTDI_ROUTING);
// 		delay_ms(1000);
// 		led_0_off();
		
		//toggleChargePump();
	}
	/* Insert application code here, after the board has been initialized. */
}

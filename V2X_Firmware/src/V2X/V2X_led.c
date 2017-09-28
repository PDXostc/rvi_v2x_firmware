/*
 * V2X_led.c
 *
 * Created: 2/12/2016 11:02:25 AM
 *  Author: Jesse Banks
 */ 

#include "V2X.h"

struct pwm_config red_pwm_cfg, green_pwm_cfg, blue_pwm_cfg ;

void led_update(void){
#ifdef LED_IO
	if ( PWR_query(1<<ENABLE_5V0B) ) { //if the host is on, assume as high power indicator
		if (simcard_status() == true)							{led_red_on();	}
		else													{led_red_off();	}
		if (ioport_get_pin_level(EXT1_PIN_SIM_NETWORK) == true)	{led_green_on();	}
		else													{led_green_off();	}
		if (ioport_get_pin_level(EXT1_PIN_SIM_PWR) == true)		{led_blue_on();	}
		else													{led_blue_off();	}
	} else {led_red_off(); led_green_off(); led_blue_off();}
#endif

#ifdef LED_PWM
	if ( PWR_query(1<<ENABLE_5V0B) ) { //if the host is on, assume as high power indicator
		if (simcard_status() == true)							{pwm_start(&red_pwm_cfg, LED_MAX_RED);	}
		else													{pwm_start(&red_pwm_cfg, LED_DIM);}
		if (ioport_get_pin_level(EXT1_PIN_SIM_NETWORK) == true)	{pwm_start(&green_pwm_cfg, LED_MAX_GREEN);	}
		else													{pwm_start(&green_pwm_cfg, LED_DIM);}
		if (ioport_get_pin_level(EXT1_PIN_SIM_PWR ) == true)	{pwm_start(&blue_pwm_cfg, LED_MAX_BLUE);	}
		else													{pwm_start(&blue_pwm_cfg, LED_DIM);}
		} else {pwm_start(&red_pwm_cfg, LED_DIM); pwm_start(&green_pwm_cfg, LED_DIM); pwm_start(&blue_pwm_cfg, LED_DIM);}

#endif
}

void led_init (void) {  
#ifdef LED_IO
	ioport_configure_pin(LED_0_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
	ioport_configure_pin(LED_1_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
	ioport_configure_pin(LED_2_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
#endif

#ifdef LED_PWM
	//setup PWM
	pwm_init(&red_pwm_cfg, PWM_TCE0, PWM_CH_C, LED_PWM_FREQ);
	pwm_init(&green_pwm_cfg, PWM_TCE0, PWM_CH_B, LED_PWM_FREQ);
	pwm_init(&blue_pwm_cfg, PWM_TCE0, PWM_CH_D, LED_PWM_FREQ);
	
	pwm_start(&red_pwm_cfg,   LED_DIM); 
	pwm_start(&green_pwm_cfg, LED_DIM); 
	pwm_start(&blue_pwm_cfg,  LED_DIM);
#endif	
}

void flash_red (void) {
	pwm_start(&red_pwm_cfg, LED_FLASH_RED);
}
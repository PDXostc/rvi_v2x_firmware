/*
 * V2X_reset.c
 *
 * Created: 9/20/2017 10:51:57 PM
 *  Author: Jesse Banks
 */ 
#include "V2X.h"

uint8_t reset_flags = RESET_NONE;

void reset_processor(void) {
	if (reset_flags) {
		if (reset_flags & (1<<RESET_SYSTEM)) {
			usb_tx_string_P(PSTR("V2X restarting\rReboot in 3 seconds\r\n>"));
			delay_s(3);
			// use write protected inteface to software reset
			ccp_write_io((uint8_t *)&RST.CTRL, RST_SWRST_bm);
		}
		// Trying to implement some kind of USB reset that won't bring down the house but might keep
		// control port happy on Linux...
		if (reset_flags & (1<<RESET_USB))
		{
			usb_tx_string_P(PSTR("::Reset USB Called::\r\n"));
			udd_detach();
			delay_s(1);
			udd_attach();
			reset_flags &= ~(1<<RESET_USB);
		}
		if (reset_flags & (1<<RESET_CAN)) {
			menu_send_CTL();
			usb_tx_string_P(PSTR("CAN restarting\r\n>"));
			PWR_can_stop();
			delay_ms(100);
			CAN_elm_init();
			reset_flags &= ~(1<<RESET_CAN);
		}
		if (reset_flags & (1<<RESET_GSM)) {
			menu_send_CTL();
			usb_tx_string_P(PSTR("GSM restarting\r\n>"));
			// Forces reset of GSM
			PWR_gsm_reset();
			delay_ms(500);
			GSM_modem_init();
			reset_flags &= ~(1<<RESET_GSM);
		}
		if (reset_flags & ~((1<<RESET_SYSTEM)|(1<<RESET_USB)|(1<<RESET_CAN)|(1<<RESET_GSM))) {
			reset_flags = reset_flags & ~((1<<RESET_SYSTEM)|(1<<RESET_USB)|(1<<RESET_CAN)|(1<<RESET_GSM));
		}
	}

}

void reset_trigger_USB (void) {
	reset_flags = reset_flags | (1<<RESET_USB);
}

void reset_trigger_SYSTEM (void) {
	reset_flags = reset_flags | (1<<RESET_SYSTEM);
}

void reset_trigger_CAN (void) {
	reset_flags = reset_flags | (1<<RESET_CAN);
}

void reset_trigger_GSM (void) {
	reset_flags = reset_flags | (1<<RESET_GSM);
}
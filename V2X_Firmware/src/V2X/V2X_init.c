/**
 * /file  V2X_init.c
 *
 * /brief V2X board initialization
 *
 * Author: Jesse Banks (jbanks2)
 **/
#include "V2X.h"

uint8_t reset_flags = RESET_NONE;

void pin_init(void)
{
		ioport_configure_pin(EXT1_PIN_HUB_STATUS			, IOPORT_DIR_INPUT						);
		ioport_configure_pin(EXT1_PIN_HOST_SHORT_CIRCUIT	, IOPORT_DIR_INPUT						);

		ioport_configure_pin(EXT1_PIN_CAN_TXD				, IOPORT_DIR_INPUT						);
		ioport_configure_pin(EXT1_PIN_CAN_RXD				, IOPORT_DIR_INPUT						);

		ioport_configure_pin(EXT1_PIN_SPI_SS				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	); //set slave select high
		ioport_configure_pin(EXT1_PIN_SPI_MOSI				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
		ioport_configure_pin(EXT1_PIN_SPI_MISO				, IOPORT_DIR_INPUT						);
		ioport_configure_pin(EXT1_PIN_SPI_SCK				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);

		ioport_configure_pin(EXT1_PIN_ACL_INT2				, IOPORT_DIR_INPUT  | IOPORT_PULL_UP	);
		ioport_configure_pin(EXT1_PIN_ACL_INT1				, IOPORT_DIR_INPUT  | IOPORT_PULL_UP	);

		ioport_configure_pin(EXT1_PIN_SR_CLEAR				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
		ioport_configure_pin(EXT1_PIN_SR_LATCH				, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);

		ioport_configure_pin(EXT1_PIN_SIM_WAKE				, IOPORT_DIR_INPUT	  					);
		ioport_configure_pin(EXT1_PIN_SIM_NETWORK			, IOPORT_DIR_INPUT	  					);

		ioport_configure_pin(EXT1_PIN_SIM_PWR				, IOPORT_DIR_INPUT						);
		ioport_configure_pin(EXT1_PIN_SEQ_RXD				, IOPORT_DIR_INPUT						);
		ioport_configure_pin(EXT1_PIN_SEQ_TXD				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);

//high here turns leds off because of inverse logic in rev 2.0
		ioport_configure_pin(LED_0_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
		ioport_configure_pin(LED_1_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
		ioport_configure_pin(LED_2_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);

		ioport_configure_pin(BUTTON_0_PIN					, IOPORT_DIR_INPUT  | IOPORT_SENSE_FALLING);
		ioport_configure_pin(BUTTON_1_PIN					, IOPORT_DIR_INPUT  | IOPORT_PULL_UP	);  //NON_REM[1] USB hub Boot Strapping option

#if V2X_REV >= REV_20
	/* 3v3 pin init, low by default... */
	ioport_configure_pin(PWR_3V3_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
#endif

}

void enable_v2x_interrupts(void) {
	// PORTA.PIN0CTRL = (PORT_ISC_RISING_gc); // set port to sense rising edge
	PORTA.INT0MASK = (0x01);			   // select PA0 for INT source
	PORTA.INTCTRL |= PORT_INT0LVL_MED_gc;  // enable interrupt 0 at medium
}

/* This function is meant to contain board-specific initialization code
 * for, e.g., the I/O pins. The initialization can rely on application-
 * specific board configuration, found in conf_board.h.
 */
void v2x_board_init(void)
{
	irq_initialize_vectors();
	sysclk_init();							//configure clock sources for core and USB
	sleepmgr_init();						// Initialize the sleep manager
	ioport_init();							//Initializes the IOPORT service
	pin_init();								//whole chip pin init, modes and initial conditions
	spi_start();							//start SPI driver
	PWR_init();								//sets SR to default states - holds power up
	enable_v2x_interrupts();
	pmic_init();
	cpu_irq_enable();
	eeprom_init();							//verifies eeprom safe for use
	menu_init();							//loads menu settings
	time_init();							//starts the RTC
	button_init();							//init button stuffs
	ACL_init();								//configures, but does not start sampling
	GSM_usart_init();						//starts direct serial channel to the SIM module
	CAN_uart_start();						//starts direct serial channel to the ELM module
	udc_start();							//start stack and vbus monitoring

	//autostart all systems
	delay_ms(500);
	GSM_modem_init();
	CAN_elm_init();
	ACL_set_sample_on();
	PWR_host_start();
#ifdef GPS_AUTO
	//send gps auto start command
	GSM_command_enable_gps_auto(1);
#endif
}

void reset_processor(void) {
	if (reset_flags) {
		if (reset_flags & (1<<RESET_SYSTEM)) {
			usb_tx_string_P(PSTR("V2X restarting\rPlease close this window\r>"));
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
			usb_tx_string_P(PSTR("CAN restarting\r>"));
			PWR_can_stop();
			delay_ms(100);
			CAN_elm_init();
			reset_flags &= ~(1<<RESET_CAN);
		}
		if (reset_flags & (1<<RESET_GSM)) {
			menu_send_CTL();
			usb_tx_string_P(PSTR("GSM restarting\r>"));
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
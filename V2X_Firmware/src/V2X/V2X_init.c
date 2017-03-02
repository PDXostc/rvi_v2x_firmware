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
		ioport_configure_pin(EXT1_PIN_HUB_SUSPEND			, IOPORT_DIR_INPUT						);	//NON_REM[0] USB hub Boot Strapping option, strapped by resistor
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

		ioport_configure_pin(EXT1_PIN_HUB_SDA				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);  //1 for NON_REM[1] USB hub Boot Strapping option
		ioport_configure_pin(EXT1_PIN_HUB_SCL				, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);	//0 for CFG_SEL USB hub Boot Strapping option

		ioport_configure_pin(EXT1_PIN_ACL_RXD				, IOPORT_DIR_INPUT						);
		ioport_configure_pin(EXT1_PIN_ACL_TXD				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);

		ioport_configure_pin(EXT1_PIN_SIM_WAKE				, IOPORT_DIR_INPUT	  					);
		ioport_configure_pin(EXT1_PIN_SIM_NETWORK			, IOPORT_DIR_INPUT	  					);

		//ioport_configure_pin(EXT1_PIN_SIM_TXD				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
		//ioport_configure_pin(EXT1_PIN_SIM_RXD				, IOPORT_DIR_INPUT						);

		ioport_configure_pin(EXT1_PIN_SIM_PWR				, IOPORT_DIR_INPUT	  					);
		ioport_configure_pin(EXT1_PIN_SEQ_RXD				, IOPORT_DIR_INPUT						);
		ioport_configure_pin(EXT1_PIN_SEQ_TXD				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);

		ioport_configure_pin(LED_0_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);
		ioport_configure_pin(LED_1_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);
		ioport_configure_pin(LED_2_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);

		ioport_configure_pin(BUTTON_0_PIN					, IOPORT_DIR_INPUT   					);
		ioport_configure_pin(BUTTON_1_PIN					, IOPORT_DIR_INPUT  | IOPORT_PULL_UP	);  //NON_REM[1] USB hub Boot Strapping option

		ioport_configure_pin(BUF0_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);	//blocks atmel serial path to CAN through buffer
		ioport_configure_pin(BUF1_PIN						, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);	//enables FTDI serial path to CAN through buffer

	#if V2X_REV <= REV_12
		/* Revision 1.2:
		 * Use charge pump for powering leds
		 */
		ioport_configure_pin(CHARGEPUMP_0_PIN				, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);
	#endif
}

/* This function is meant to contain board-specific initialization code
 * for, e.g., the I/O pins. The initialization can rely on application-
 * specific board configuration, found in conf_board.h.
 */
void v2x_board_init(void)
{
	irq_initialize_vectors();
	pmic_init();
	sysclk_init();							//configure clock sources for core and USB
	sleepmgr_init();						// Initialize the sleep manager
	ioport_init();							//Initializes the IOPORT service
	pin_init();								//whole chip pin init, modes and initial conditions
	spi_start();							//start SPI driver
	PWR_init();								//sets SR to default states - holds power up
	cpu_irq_enable();
	eeprom_init();							//verifies eeprom safe for use
	menu_init();							//loads menu settings
	time_init();							//starts the RTC
	button_init();							//init button stuffs
	ACL_init();								//configures, but does not start sampling
	GSM_usart_init();						//starts direct serial channel to the SIM module
	CAN_uart_start();						//starts direct serial channel to the ELM module
	canbus_serial_routing(AVR_ROUTING);		//cause the serial 3-state buffer to route the serial path from the ELM to the FTDI 
	udc_start();							//start stack and vbus monitoring
	PWR_hub_start();						//connect the hub to the computer

	//autostart all systems
	delay_ms(500);
	GSM_modem_init();
	CAN_elm_init();
	ACL_set_sample_on();
	PWR_host_start();
}

void reset_processor(void) {
	if (reset_flags) {
		if (reset_flags & (1<<RESET_SYSTEM)) {
			usb_tx_string_P(PSTR("V2X restarting\rPlease close this window\r>"));
			delay_s(3);
			CCP = 0xd8; //enable write protected registers
			RST_CTRL = true; //force SW reset
		}
		if (reset_flags & (1<<RESET_USB)) {
			usb_tx_string_P(PSTR("USB restarting\r>"));
			delay_s(3);
			PWR_hub_stop();
			delay_s(7);
			PWR_hub_start();
			delay_s(1);
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
			PWR_gsm_stop();
			delay_ms(100);
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
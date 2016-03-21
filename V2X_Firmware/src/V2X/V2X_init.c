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

		ioport_configure_pin(CHARGEPUMP_0_PIN				, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW	);
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
	power_control_init();					//sets SR to default states - holds power up
	cpu_irq_enable();
	time_init();
	button_init();							//init button stuffs
	ACL_init();								//configures, but does not start sampling
	GSM_usart_init();
	CAN_uart_start();
	canbus_serial_routing(AVR_ROUTING);		//cause the serial 3-state buffer to route the serial path from the ELM to the FTDI 
	udc_start();							//start stack and vbus monitoring
	power_hub_start();						//connect the hub to the computer
}

void reset_processor(void) {
	if (reset_flags != 0) {
		switch (reset_flags) {
		case RESET_SYSTEM:
			usb_tx_string_P(PSTR("V2X restarting\rPlease close this window\r>"));
			delay_s(10);
			CCP = 0xd8; //enable write protected registers
			RST_CTRL = true; //force SW reset
			break;
		case RESET_USB:
			usb_tx_string_P(PSTR("USB restarting"));
			delay_s(10);
			power_hub_stop();
			delay_s(7);
			power_hub_start();
			delay_s(1);
			break;
		case RESET_CAN:
			usb_tx_string_P(PSTR("ELM restarting"));
			CAN_restart();
			break;
		case RESET_SIM:
			power_sim_stop();
			power_sim_start();
			break;
		default:
			break;
		}
	}
	reset_flags = RESET_NONE;
}

void reset_trigger_USB (void) {
	reset_flags = RESET_USB;
}

void reset_trigger_SYSTEM (void) {
	reset_flags = RESET_SYSTEM;
}

void reset_trigger_CAN (void) {
	reset_flags = RESET_CAN;
}

void reset_trigger_SIM (void) {
	reset_flags = RESET_SIM;
}
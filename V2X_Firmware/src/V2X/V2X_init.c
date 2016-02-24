/**
 * /file  V2X_init.c
 *
 * /brief V2X board initialization
 *
 * Author: Jesse Banks (jbanks2)
 **/
#include "V2X.h"

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

		ioport_configure_pin(EXT1_PIN_SIM_WAKE				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
		ioport_configure_pin(EXT1_PIN_SIM_NETWORK			, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);

		ioport_configure_pin(EXT1_PIN_SIM_TXD				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
		ioport_configure_pin(EXT1_PIN_SIM_RXD				, IOPORT_DIR_INPUT						);

		ioport_configure_pin(EXT1_PIN_SIM_PWR				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
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
	cpu_irq_enable();
	sleepmgr_init();						// Initialize the sleep manager
	sysclk_init();							//configure clock sources for core and USB
	ioport_init();							//Initializes the IOPORT service
	pin_init();								//whole chip pin init, modes and initial conditions
	spi_start();							//start SPI driver
	power_control_init();					//sets SR to default states - holds power up
	ACL_init();					//configures, but does not start sampling
	canbus_serial_routing(AVR_ROUTING);		//cause the serial 3-state buffer to route the serial path from the ELM to the FTDI 
	udc_start();							//start stack and vbus monitoring
}

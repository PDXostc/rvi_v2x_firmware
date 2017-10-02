/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "../../../V2X/V2X.h"
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	ioport_configure_pin(EXT1_PIN_HUB_STATUS			, IOPORT_DIR_INPUT						);
	ioport_configure_pin(EXT1_PIN_HOST_SHORT_CIRCUIT	, IOPORT_DIR_INPUT						);

	ioport_configure_pin(EXT1_PIN_CAN_RXD				, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);
	ioport_configure_pin(EXT1_PIN_CAN_TXD				, IOPORT_DIR_INPUT						);

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

	ioport_configure_pin(BUTTON_0_PIN					, IOPORT_DIR_INPUT  | IOPORT_SENSE_FALLING);
	ioport_configure_pin(BUTTON_1_PIN					, IOPORT_DIR_INPUT  | IOPORT_PULL_UP	);  //NON_REM[1] USB hub Boot Strapping option

	/* 3v3 pin init, low by default... */
	ioport_configure_pin(PWR_3V3_PIN					, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH	);

	irq_initialize_vectors();
	sysclk_init();							//configure clock sources for core and USB
	//sleepmgr_init();						// Initialize the sleep manager
	spi_start();							//start SPI driver
	PWR_init();								//sets SR to default states - holds power up
 	pmic_init();
 	cpu_irq_enable();
	PWR_mode_low();
	led_init();

	PORTA.INT0MASK = 0b000001001;			// select PA0 and PA3 for INT source, button and usb-vbus
//	PORTA.INT0MASK = 0b000000001;			// select PA0 for INT source, button
	PORTA.INTCTRL |= PORT_INT0LVL_MED_gc;	// enable interrupt 0 at medium
	
 	eeprom_init();							//verifies eeprom safe for use
 	job_timeout_init();						//clears active job timers, must happen before CSC_init
 	menu_init();							//loads menu settings
 	time_init();							//starts the RTC
 	button_init();							//init button stuffs
 	ACL_init();								//configures, but does not start sampling
 	GSM_uart_start();						//starts direct serial channel to the SIM module
 	CAN_uart_start();						//starts direct serial channel to the ELM module
 	udc_start();							//start stack and vbus monitoring
 	USB_vbus_mount();						//check vbus mount if req.
	CSC_init();								//gets eeprom saved enable states
	 
}


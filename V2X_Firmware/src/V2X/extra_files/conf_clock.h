/**
 * \file clock_conf.h
 *
 * \brief Initial firmware for V2X bring up
 *
 * Author: Jesse Banks (jbanks2)
 *
 * Usage: move this file to src/config/clock_conf.h by replacing the existing file.
 */

#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED



#define V2X_CLOCK_DEF

//#include "../V2X/V2X_board.h"

#define CONFIG_SYSCLK_SOURCE				SYSCLK_SRC_PLL
#define CONFIG_SYSCLK_PSADIV				SYSCLK_PSADIV_1
#define CONFIG_SYSCLK_PSBCDIV				SYSCLK_PSBCDIV_1_1
#define CONFIG_PLL0_SOURCE					PLL_SRC_RC2MHZ
#define CONFIG_PLL0_MUL						(16)
#define CONFIG_PLL0_DIV						(1)

#define CONFIG_USBCLK_SOURCE				USBCLK_SRC_RCOSC
#define CONFIG_OSC_RC32_CAL					48000000UL
#define CONFIG_OSC_AUTOCAL_RC32MHZ_REF_OSC  OSC_ID_USBSOF

#define CONFIG_RTC_SOURCE					SYSCLK_RTCSRC_TOSC

#endif /* CONF_CLOCK_H_INCLUDED */
/*
 * V2X_menu.c
 *
 * Created: 2/23/2016 9:00:14 AM
 *  Author: Jesse Banks
 */

#include <errno.h>
#include "V2X.h"
#include "ctype.h"

char CMD_buffer [30] = "\0";
uint8_t sample[6] = {0,0,0,0,0,0};
Bool verbose = true;

void menu_init(void) {
	verbose = nvm_eeprom_read_byte(EE_verbose);
}

void menu_add_to_command(char value) {
	if (value == 8 || value == 0x7f) {//if backspace or delete
		int leng = strlen(CMD_buffer); //get buffer sting length
		if (leng > 0) {	//if length not 0
			CMD_buffer[leng-1] = '\0';	//null out the last char
		}
	} else if (value > 0x20 && value < 0x7E ) {		//if in ascii set
		char char_str[2] = {'?','\0'};
		char_str[0] = value;
		strcat(CMD_buffer, char_str);					//store to buffer

	}
}

void menu_main(void) {
	menu_send_n(); //to preserve the input command in the terminal for the 
	for(int i = 0; i < 4; i++) { // Only lower case the first 4 characters, because some of the CAN functions take in strings which ARE case-sensitive
		CMD_buffer[i] = tolower(CMD_buffer[i]);
	}
	if (CMD_buffer[0] == 'v' && CMD_buffer[1] == 'x') {
		switch(CMD_buffer[2]) {
			case 'i': //information
				USB_tx_string_P(PSTR("Vehicle to Everything (V2X) RVI Node 2016 -2017\r\nOpen source hardware and software\r\nHW: "));
				USB_send_string(USB_CMD, BOARD_NAME);
				USB_tx_string_P(PSTR("\r\nFW: "));
				USB_send_string(USB_CMD, V2X_FW_REV);
				break;
//			case 'j': //Jaguar logo
//				USB_tx_string_P(PSTR("\r\n\r\n   ,ggp@@@@mgg,,\r\n,$Q$(`S@@$;g$$$$$@$@@gg,\r\n;gP'$@gg)$$@@$@@@$(L$$||$$@g,\r\n  `g$P`  ``*%@@@P`)Eg|||lLLL||$Bgg,\r\n    `       ,gg$$@gg,`$..``$Z$$$$$EB$$@g,\r\n         @P`pgg$$$||`)gggg;,,     |$$$|$$$@g,\r\n         9w&    '*^^` ``*P#9BB00000$$$@|`$$$g|Sg,\r\n                                    *$@@L ```T$W~)%g,\r\n                                      *%@gg,,,,,    5/Sw,     ,\r\n                                          ```` ` `9g `9g,``*^|'\r\n                                                    `#g,`)h\r\n\r\n   Developed at Jaguar Land Rover OSCT. Portland OR 2016"));
//				break;
			case 'g'://GENIVI logo
				USB_tx_string_P(PSTR("GENIVI 2017"));
				break;
			case 'q':
				menu_status();
				break;
			case 'r':  //reset
				reset_trigger_SYSTEM();
				break;
			case 'a':  //accel
				menu_accel();
				break;
			case 'c':  //CAN interface
				menu_can();
				break;
			case 'm':  //modem
				menu_modem();
				break;
			case 'p':  //power
				menu_power();
				break;
			case 't':  //timer functions
				menu_timer();
				break;
			case 's':  //sleep-state check functions
				menu_sleep();
				break;
			case 'v': //toggle verbose setting
				verbose = !verbose;
				nvm_eeprom_write_byte(EE_verbose,menu_is_verbose());
				USB_tx_string_PV(PSTR("Verbose is ON"));
				break;
			case '?':  //timer functions
			default:
				USB_tx_string_P(PSTR("*** Main Menu ***\r\nI: Device information\r\nA: Accelerometer menu(ACL)\r\nC: ELM327 menu(CAN)\r\nM: SIM Modem menu(GSM)\r\nP: Power menu\r\nT: Time (RTC clock) menu\r\nS: Sleep-state timer checks menu\r\nQ: Query system status\r\nV: Toggle verbose messages\r\nR: Reboot"));
				break;
			}
	}else{
			USB_tx_string_PV(PSTR("All commands start VX"));
	}
	clear_buffer(CMD_buffer);	//clear the buffer for next command
	menu_send_n_st();
}

void menu_accel (void) {
	int speed, data16;
	switch (CMD_buffer[3]) {
	case 'd':  //disable
		ACL_set_sample_off();
		USB_tx_string_PV(PSTR("Accelerometer is off"));
		break;
	case 'e':  //enable
		ACL_set_sample_on();
		USB_tx_string_PV(PSTR("Accelerometer started"));
		break;
	case 'i':
		USB_tx_string_P(PSTR("V2X uses the ADXL345 accelerometer from Analog Devices"));
		break;
	case 'q':
		menu_accel_status();
		break;
	case 'r':  //restart
		ACL_set_sample_off();
		ACL_take_sample(sample); //pushes "off" command
		ACL_init(); //
		ACL_set_sample_on();
		ACL_take_sample(sample); //pushes init
		USB_tx_string_PV(PSTR("Accelerometer has been restarted"));
		break;
	case 's':  //sample rate
 		speed = atoi(CMD_buffer+4);
		if (speed > 3199) {
			ACL_set_rate(ACL_RATE_3200);
			USB_tx_string_PV(PSTR("Set to 3200Hz"));
		}else if (speed > 1599) {
			ACL_set_rate(ACL_RATE_1600);
			USB_tx_string_PV(PSTR("Set to 1600Hz"));
		}else if (speed > 799) {
			ACL_set_rate(ACL_RATE_800);
			USB_tx_string_PV(PSTR("Set to 800Hz"));
		}else if (speed > 399) {
			ACL_set_rate(ACL_RATE_400);
			USB_tx_string_PV(PSTR("Set to 400Hz"));
		}else if (speed > 199) {
			ACL_set_rate(ACL_RATE_200);
			USB_tx_string_PV(PSTR("Set to 200Hz"));
		}else if (speed > 99) {
			ACL_set_rate(ACL_RATE_100);
			USB_tx_string_PV(PSTR("Set to 100Hz"));
		}else if (speed > 49) {
			ACL_set_rate(ACL_RATE_50);
			USB_tx_string_PV(PSTR("Set to 50Hz"));
		}else if (speed > 24) {
			ACL_set_rate(ACL_RATE_25);
			USB_tx_string_PV(PSTR("Set to 25Hz"));
		}else if (speed > 11) {
			ACL_set_rate(ACL_RATE_12);
			USB_tx_string_PV(PSTR("Set to 12Hz"));
		}else if (speed > 5) {
			ACL_set_rate(ACL_RATE_6);
			USB_tx_string_PV(PSTR("Set to 6Hz"));
		}else if (speed > 2) {
			ACL_set_rate(ACL_RATE_3);
			USB_tx_string_PV(PSTR("Set to 3Hz"));
		}else if (speed > 0) {
			ACL_set_rate(ACL_RATE_1);
			USB_tx_string_PV(PSTR("Set to 1Hz"));
 		}else {
			USB_tx_string_P(PSTR("ERROR: Out of range, try again."));
			menu_send_n();
		}
		break;
	case 'w':
		speed = atoi(CMD_buffer+4);
		if (speed > 16) {speed = 0;}
		if (speed > 8) {
			ACL_set_range(ACL_16G_RANGE);
			USB_tx_string_PV(PSTR("+/-16G Range"));
		}else if (speed > 4) {
			ACL_set_range(ACL_8G_RANGE);
			USB_tx_string_PV(PSTR("+/-8G Range"));
		}else if (speed > 2) {
			ACL_set_range(ACL_4G_RANGE);
			USB_tx_string_PV(PSTR("+/-4G Range"));
		}else if (speed > 0) {
			ACL_set_range(ACL_2G_RANGE);
			USB_tx_string_PV(PSTR("+/-2G Range"));
		}else {menu_send_out_of_range();}
		break;

	case 'x':
	case 'y':
	case 'z':
		data16 = atoi(CMD_buffer+4);  //convert input chars to int
		if (data16 == 0){ //conversion resulted in 0
			if (CMD_buffer[4] != '0'){ //check if it is really a '0' of an invalid conversion
				menu_send_out_of_range(); //invalid result
				break; //exit offset menu
			}
		}
		if (data16 < -127 || data16 > 127) {  //must be 16 to detect out of range
			menu_send_out_of_range();
		} else {
			uint8_t data8 = (uint8_t)data16;  //reduce to 8 bits of data
			switch (CMD_buffer[3]) {
				case 'x':
					if (menu_is_verbose()) {USB_tx_string_P(PSTR("X"));}
						ACL_set_offset(ACL_X_OFFSET, data8);
					break;
				case 'y':
					if (menu_is_verbose()) {USB_tx_string_P(PSTR("Y"));}
						ACL_set_offset(ACL_Y_OFFSET, data8);
					break;
				case 'z':
					if (menu_is_verbose()) {USB_tx_string_P(PSTR("Z"));}
						ACL_set_offset(ACL_Z_OFFSET, data8);
					break;
				default:
					menu_send_q();
					break;
			}
			USB_tx_string_PV(PSTR(" Offset: "));
			if (menu_is_verbose()) {
				menu_print_int(data16);
			}
		}
		break;
	case 'g':
		if (ACL_sampling()){
			if (USB_port_is_active(USB_ACL)) {
				ACL_get_last_sample(sample);
			} else {
				ACL_take_sample(sample);
			}
			ACL_data_to_string(sample, CMD_buffer);
			for (int i = 0; i < strlen(CMD_buffer); i++)
				{USB_send_char(USB_CMD, CMD_buffer[i]);}
		} else {
			USB_tx_string_P(PSTR("ERROR: Not currently sampling, use VXAE"));
 		}
		break;
	case 'f':
		if (CMD_buffer[4] == '0') {
				ACL_set_full_resolution(0);
			USB_tx_string_PV(PSTR("10bit resolution, scale changes with 'G' range"));
		}
		else if (CMD_buffer[4] == '1') {
				ACL_set_full_resolution(1);
			USB_tx_string_PV(PSTR("Full resolution, 4mg/bit"));
		}
		else {menu_send_q();}
		break;
	case '?':
	default:
		USB_tx_string_P(PSTR("*** Accelerometer Menu ***\r\nE: Enable\r\nD: Disable\r\nR: Restart\r\nI: Subsystem Information\r\nQ: Query status\r\nG: Get last Sample\r\nSn: Set sample rate in HZ (1, 3, 6, 12, 25, 50, 100, 200, 400, 800)\r\nWn: Set \"G\" Range (2, 4, 8, 16)\r\nFn: Full range (1: full 0: 10bit)\r\nXn: Set X offset (+/-127)\r\nYn: Set Y offset\r\nZn: Set Z offset"));
		break;
	}
}

void menu_modem (void) {
	switch (CMD_buffer[3]) {
	case 'd':
		PWR_gsm_stop(); //disable
		USB_tx_string_PV(PSTR("GSM shutdown"));
		break;
	case 'e':  //enable modem
		USB_tx_string_PV(PSTR("GSM Start pending"));
			GSM_modem_init();
		break;
	case 'r':  //reset modem
		reset_trigger_GSM();
		//USB_tx_string_PV(PSTR("GSM RESET"));
		break;
	case 'g':
		USB_tx_string_PV(PSTR("Enable GPS"));
		GSM_command_enable_gps_auto(true);
		break;
	case 'q':
		menu_modem_status();
		break;
	case 'i':
		USB_tx_string_P(PSTR("V2X uses the SIM5320A 3G or SIM7100a 4G GSM modem + GPS receiver by SIMCOM"));
		break;
	case 'x':
	// FIXME: please verify that the newline is actually warranted here
		strcat_P(CMD_buffer, PSTR("\r"));  //put these char at the end of the string
		USB_send_string(USB_CMD, &CMD_buffer[4]);
		GSM_add_string_to_buffer(BUFFER_OUT, &CMD_buffer[4]); //send it on to the modem
		GSM_mark_for_processing(BUFFER_OUT); //initiate send
 		break;
	case '?':
	default:
		USB_tx_string_P(PSTR("*** Modem Menu ***\r\nE: Enable\r\nD: Disable\r\nG: Enable GPS\r\nR: RESET. Emergency use.\r\nI: Subsystem Information\r\nQ: Query status\r\nXt: Comand Pass through"));
		break;
	}
}

void menu_can (void) {
	char buffer[EE_CAN_ARRAY_SIZE+1];
	switch (CMD_buffer[3]) {
	case 'd':  //disable;
		USB_tx_string_PV(PSTR("CAN is off"));
		PWR_can_stop();
		PWR_is_5_needed();
		break;
	case 'e':  //enable
		USB_tx_string_PV(PSTR("CAN Starting\n"));
		CAN_elm_init();
		break;
	case 'r':  //reset
		//USB_tx_string_PV(PSTR("CAN restarting"));
		reset_trigger_CAN();
		break;
	case 'q': //query
		menu_can_status();
		break;
	case 'i': //info
		USB_tx_string_P(PSTR("V2X uses the STN1110 CANbus interface from Scantool\r\nThe STN1110 is compliant with the ELM327 V1.3"));
		break;
	case 'x': //pass command
		strcat_P(CMD_buffer, PSTR("\r"));
		CAN_add_string_to_buffer(BUFFER_OUT, CMD_buffer+4);
		CAN_mark_for_processing(BUFFER_OUT);
		CAN_start_snoop();
		break;
	case 's':  //save string to eeprom
		//strcat_P(CMD_buffer, PSTR("\r\n"));
		eeprom_store_CAN_string(CMD_buffer+4);
		break;
	case 'w': //what is stored in EE?
 		eeprom_read_CAN_string(buffer);
 		USB_send_string(USB_CMD, buffer);
		break;
	case 'a': //load string and parse from eeprom
		CAN_EE_start();
		break;
	case '?':
	default:
		USB_tx_string_P(PSTR("*** CANbus Menu ***\r\nE: Enable\r\nD: Disable\r\nR: Restart\r\nSt: Store config string to EE\r\nA: Execute config string in EE\r\nW: Display config string in EE\r\nI: Subsystem Information\r\nQ: Query status\r\nXt: Command pass through"));
		break;
	}
}

void menu_power (void) {
    char printBuffer[200];

	CMD_buffer[4] = tolower(CMD_buffer[4]);
	
	switch (CMD_buffer[3]) {
	case 'd':  //disable power
		switch (CMD_buffer[4]) {
		case '3':  //4v
			USB_tx_string_PV(PSTR("Disabling 3V supply"));
			PWR_3_stop();
			break;
		case '4':  //4v
			USB_tx_string_PV(PSTR("Disabling 4V supply"));
			PWR_4_stop();
			break;
		case '5':  //5v
			USB_tx_string_PV(PSTR("Disabling 5V supply"));
			PWR_5_stop();
			break;
		case 'h':  //host
			USB_tx_string_PV(PSTR("Disabling Host power supply"));
			PWR_host_stop();
			break;
		case 'a': // disable all. shutdown.
			PWR_shutdown();
			break;
		default:
			menu_send_q();
			break;
		}
		break; //disable power
	case 'e':  //enable power
		switch (CMD_buffer[4]) {
		case '3':  //4v
			USB_tx_string_PV(PSTR("Enabling 3V supply"));
			PWR_3_start();
			break;
		case '4':  //4v
			USB_tx_string_PV(PSTR("Enabling 4V supply"));
			PWR_4_start();
			break;
		case '5':  //5v
			USB_tx_string_PV(PSTR("Enabling 5V supply"));
			PWR_5_start();
			break;
		case 'h':  //host
			USB_tx_string_PV(PSTR("Enabling Host power supply"));
			PWR_host_start();
			break;
		default:
			menu_send_q();
			break;
		}
		break;  //enable power
	case 'r':
		PWR_turn_off(~POWER_CONTROL_DEFAULT_VALUE);
		PWR_turn_on(POWER_CONTROL_DEFAULT_VALUE);
		PWR_push();
		menu_send_ok();
		break;
    case 'y':
        if (menu_is_verbose()) {
            switch (PWR_get_wake_up_reason()) {
                case PWR_WAKE_UP_REASON_UNKNOWN:
                    	snprintf(printBuffer, 200, "Wake up reason: %u - %s", PWR_get_wake_up_reason(), "UNKNOWN");
                    break;
                case PWR_WAKE_UP_REASON_BUTTON:
                    	snprintf(printBuffer, 200, "Wake up reason: %u - %s", PWR_get_wake_up_reason(), "BUTTON PRESS");
                    break;
                case PWR_WAKE_UP_REASON_CAR_RUNNING:
                    	snprintf(printBuffer, 200, "Wake up reason: %u - %s", PWR_get_wake_up_reason(), "CAR RUNNING");
                    break;
            }
        } else {
            snprintf(printBuffer, 200, "%u", PWR_get_wake_up_reason());
        }

        USB_send_string(USB_CMD, printBuffer);

        break;
	case 'q':
		menu_power_status();
		break;
	case 'h': //switch to high power mode, all peripherals enabled
		PWR_mode_high();
		break;
	case 'l': //switch to low power mode, all peripherals disabled
		PWR_mode_low();
		break;
	case '?':
	default:
		USB_tx_string_P(PSTR("*** Power Menu ***\r\nEn: Enable power supply (3, 4, 5, H(ost) )\r\nDn: Disable power supply (3, 4, 5, H(ost), A(ll) )\r\nR: Reset to defaults\r\nQ: Query status\r\nY: Why did I wake up?\r\nH: High power mode (all peripherals enabled)\r\nL: Low power mode (all peripherals disabled)"));
		break;
	}
}

void menu_timer(void) {
	switch (CMD_buffer[3]) {
	case 's':  //set V2X time
		time_set(menu_sample_number(CMD_buffer+4));
		USB_tx_string_PV(PSTR("Time has been set"));
		break;
	case 'g':  //get V2X time
		USB_tx_string_P(PSTR("UET: "));
		menu_print_int(time_get());
		USB_tx_string_P(PSTR("\r\nTime: "));
		time_print_human_readable();
		break;
	case 'a':  //absolute alarm
		time_alarm_set(menu_sample_number(CMD_buffer+4));
		USB_tx_string_PV(PSTR("Alarm has been set"));
		break;
	case 'r':  //relative alarm
		time_alarm_set_relative(menu_sample_number(CMD_buffer+4));
		USB_tx_string_PV(PSTR("Alarm has been set"));
		break;
	case 'z':  //set time zone
		time_zone_set(menu_sample_number(CMD_buffer+4));
		USB_tx_string_P(PSTR("TZN="));
		menu_print_int(time_zone_get());
		break;
	case 'd':  //set time zone
		USB_tx_string_P(PSTR("DST="));
		if (menu_sample_number(CMD_buffer+4)) {
			time_dst_set(1);
			menu_send_1();
		} else {
			time_dst_set(0);
			menu_send_0();
		}
		break;
	case 'i':  //timer system information
		USB_tx_string_P(PSTR("The timer module uses Unix Epoch timestamps (UET) \r\nH24: clock has been set/sync within 24hrs\r\nALM: alarm is set for the future"));
		break;
	case 'q':  //timer inquery
		menu_timer_status();
		break;
	case 'u':
		//USB_tx_string_P(PSTR("GPS update"));
		GSM_time_job();
		break;
	case '?':  //Menu options
	default:
		USB_tx_string_P(PSTR("*** Timer Menu ***\r\nSn: Set V2X time (UET)\r\nDn: Daylight Savings Time\r\nG: Get V2X time\r\nAn: Set absolute alarm (UET) \r\nRn: Set relative alarm (Seconds)\r\nI: timer information\r\nQ: Timer inquery\r\nU: Update using gps\r\nZn: Set timezone"));
		break;
	}
}

void menu_sleep(void) {
	CMD_buffer[4] = tolower(CMD_buffer[4]);

    long timeoutInterval = 0;
    char printBuffer[200];

	switch (CMD_buffer[3]) {
		case 'd':  //disable sleep-state checks
			switch (CMD_buffer[4]) {
				case 'c':  // car-on check

                    CSC_disable_car_state_check();
                    USB_tx_string_PV(PSTR("Disabling car-state check"));
                    break;
					
				case 'a':  // accelerometer check
					USB_tx_string_PV(PSTR("Disabling accelerometer sleep-state check - NOT YET IMPLEMENTED"));
                    break;
					
				case 't':  // text-message check
					USB_tx_string_PV(PSTR("Disabling text-message sleep-state check - NOT YET IMPLEMENTED"));
                    break;
					
				case 'g': // gps-movement check
					USB_tx_string_PV(PSTR("Disabling gps-movement sleep-state check - NOT YET IMPLEMENTED"));
                    break;
					
				default:
					menu_send_q();
					break;
			}
			
			break; //disable sleep-state checks
		
		case 'e':  //enable sleep-state checks
			switch (CMD_buffer[4]) {
				case 'c':  // Car-on check
	                 CSC_enable_car_state_check();
                    USB_tx_string_PV(PSTR("Enabling car-state check"));
                    break;
					
				case 'a':  // Accelerometer check
					USB_tx_string_PV(PSTR("Enabling accelerometer sleep-state check - NOT YET IMPLEMENTED"));
					break;
					
				case 't':  // text-message check
					USB_tx_string_PV(PSTR("Enabling text-message sleep-state check - NOT YET IMPLEMENTED"));
					break;
				
				case 'g': // gps-movement check
					USB_tx_string_PV(PSTR("Enabling gps-movement sleep-state check - NOT YET IMPLEMENTED"));
					break;
					
				default:
					menu_send_q();
					break;
			}
			
			break;  //enable sleep-state checks

        case 't':  // set check timeout intervals
			switch (CMD_buffer[4]) {
				case 'l':

                    errno = 0;
                    timeoutInterval = strtol(&CMD_buffer[5], NULL, 10);

                    if (timeoutInterval <= 0 || timeoutInterval > 65535 || errno == ERANGE)
                        USB_tx_string_P(PSTR("Invalid interval. Value must be between 1 and 65,535."));
                    else
						eeprom_write_unsigned_int_16(EE_car_state_check_low_power_check_interval,
													 (uint16_t) timeoutInterval);

                    break;

				case 'h':

                    errno = 0;
					timeoutInterval = strtol(&CMD_buffer[5], NULL, 10);

                    if (timeoutInterval <= 0 || timeoutInterval > 65535 || errno == ERANGE)
                        USB_tx_string_P(PSTR("Invalid interval. Value must be between 1 and 65,535."));
                    else
						eeprom_write_unsigned_int_16(EE_car_state_check_high_power_check_interval,
													 (uint16_t) timeoutInterval);

                    break;

				default:
					menu_send_q();
					break;
			}

			break;  // set check timeout intervals
			
		case 'q': // query state of checks
             snprintf(printBuffer, 200, "Car-state check: %s\n\rLow-power car-state check interval: %u\n\rHigh-power car-state check interval: %u\n\r",
 											nvm_eeprom_read_byte(EE_car_state_check_enabled) == CSC_CAR_STATE_CHECK_ENABLED ? "ENABLED" : "DISABLED",
 											eeprom_read_unsigned_int_16(EE_car_state_check_low_power_check_interval),
 											eeprom_read_unsigned_int_16(EE_car_state_check_high_power_check_interval));

            USB_send_string(USB_CMD, printBuffer);

            break;
			
		case 'i': // briefly describe function of the sleep-state timer checks
            USB_tx_string_P(PSTR("Allows enable/disable and interval control of scheduled checks performed"));
            break;
				
		case '?':
		default:
			USB_tx_string_P(PSTR("*** Sleep-State Checks Menu ***\r\nE<N>: Enable Checks ([C]ar-On, [A]ccelerometer, [T]ext Messages Received, [G]PS Movement)\r\nD<N>: Disable Checks ([C]ar-On, [A]ccelerometer, [T]ext Messages Received, [G]PS Movement)\r\nT<N>XXXX: Update Check Timeout Interval ([L]ow-Power Car-State Check Interval, [H]igh-Power Car-State Check Interval)\r\nQ: Query status of checks\r\nI: Info on checks"));
			break;
	}
}

void menu_send_ok(void) {USB_tx_string_P(PSTR("OK\r\n>"));
}

void menu_send_q(void) {USB_tx_string_P(PSTR("?\r\n>"));
}

void menu_send_1(void) {USB_tx_string_P(PSTR("1\r\n"));
}

void menu_send_0(void) {USB_tx_string_P(PSTR("0\r\n"));
}

void menu_send_n(void) {USB_tx_string_P(PSTR("\r\n"));
}

void menu_send_n_st(void) {USB_tx_string_P(PSTR("\r\n>"));
}

void menu_send_out_of_range(void) {USB_tx_string_P(PSTR("ERROR: out of range\r\n>"));
}

void menu_send_GSM(void) {USB_tx_string_P(PSTR("GSM>:"));
}

void menu_send_CTL(void) {USB_tx_string_P(PSTR("CTL>:"));
}

void menu_send_CAN(void) {USB_tx_string_P(PSTR("CAN>:"));
}

void menu_send_BTN(void) {USB_tx_string_P(PSTR("BTN>:"));
}

void menu_send_CSC(void) {USB_tx_string_P(PSTR("CSC>:"));
}

// Bool menu_verbose(void) {
// 	return verbose;
// }

void menu_print_int(long value) {
	char c_buf[13];
	ltoa(value, c_buf, 10);
	int i = 0;  //clear the pointer
	while (c_buf[i] != 0)
		{USB_send_char(USB_CMD, c_buf[i++]);}
}

long menu_sample_number(char * input) {
	return atol(input);  //convert input chars to int
}

void menu_lockup (void) {
	char msg[] = "AVR>>RESET:\r\n";
	USB_send_string(USB_CMD, msg);
	USB_send_string(USB_ACL, msg);
	USB_send_string(USB_CAN, msg);
	delay_s(1);
	RST_CTRL = true; //force SW reset
}

void menu_status (void) {
	menu_power_status();
	menu_accel_status();
	menu_can_status();
	menu_modem_status();
	menu_timer_status();
}

void menu_accel_status(void) {
	USB_tx_string_P(PSTR("ACL="));
 	if (ACL_sampling())
 	{menu_send_1();}
 	else	{menu_send_0();}
}

void menu_modem_status(void) {
	USB_tx_string_P(PSTR("SIMPWR="));
 	if (sim_power_status())
	{menu_send_1();}
	else	{menu_send_0();}
	USB_tx_string_P(PSTR("SIMNET="));
	if (sim_net_status())
	{menu_send_1();}
	else	{menu_send_0();}
	USB_tx_string_P(PSTR("SIMCARD="));
	if (simcard_status())
	{menu_send_1();}
	else	{menu_send_0();}
	USB_tx_string_P(PSTR("IMEI="));
	USB_send_string(USB_CMD, GSM_get_imei());
	menu_send_n();
}

void menu_simcard_status(void){
	if (simcard_status())
		{menu_send_1();}
	else	{menu_send_0();}
}

void menu_can_status(void) {
	USB_tx_string_P(PSTR("CAN="));
	if (PWR_query((1<<ENABLE_CAN_RESET)))
			{menu_send_1();}
	else	{menu_send_0();}
}

void menu_power_status(void) {
	USB_tx_string_P(PSTR("3V3="));
	if (ioport_get_pin_level(PWR_3V3_PIN)==true)
			{menu_send_1();}
	else	{menu_send_0();}
	USB_tx_string_P(PSTR("4V1="));
	if (PWR_query((1<<ENABLE_4V1)))
			{menu_send_1();}
	else	{menu_send_0();}
	USB_tx_string_P(PSTR("5V0="));
	if (PWR_query((1<<ENABLE_5V0)))
			{menu_send_1();}
	else	{menu_send_0();}
	USB_tx_string_P(PSTR("HOST="));
	if (PWR_query((1<<ENABLE_5V0B)))
			{menu_send_1();}
	else	{menu_send_0();}
}

void menu_timer_status (void) {
	USB_tx_string_P(PSTR("TZN="));
	menu_print_int(time_zone_get());
	menu_send_n();
	USB_tx_string_P(PSTR("DST="));
	if (time_dst_get())
			{menu_send_1();}
	else	{menu_send_0();}
	USB_tx_string_P(PSTR("UTC="));
	menu_print_int(time_get());
	menu_send_n();
	USB_tx_string_P(PSTR("H24="));
	if (time_is_current())
			{menu_send_1();}
	else	{menu_send_0();}
	USB_tx_string_P(PSTR("ALM="));
	menu_print_int(time_alarm_get());
	menu_send_n();
	USB_tx_string_P(PSTR("ALS="));
	if (time_alarm_active())
			{menu_send_1();}
	else	{menu_send_0();}

}
			
/*
 * V2X_menu.h
 *
 * Created: 2/23/2016 9:00:30 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_MENU_H_
#define V2X_MENU_H_

/**
* @def menu_init
* @brief loads menu settings
**/
void menu_init(void);

/**
 * @def menu_add_to_command
 * @brief controls the add/sub from the input command buffer
 * @param value the char to be added to the buffer, can be backspace
 **/
void menu_add_to_command(char value);

/**
 * @def menu_main
 * @brief main menu content and conditional branching
 **/
void menu_main(void);

/**
 * @def menu_send_ok
 * @brief canned message, sends "OK"
 **/
void menu_send_ok (void);

/**
 * @def menu_send_q
 * @brief canned message, send '?'
 **/
void menu_send_q (void);

/**
 * @def menu_send_1
 * @brief canned message, sends '1'
 **/
void menu_send_1(void);

/**
 * @def menu_send_0
 * @brief canned message, sends '0'
 **/
void menu_send_0(void);

/**
 * @def menu_send_n
 * @brief canned message, sends new line char
 **/
void menu_send_n(void);

/**
 * @def menu_send_n_st
 * @brief canned message, sends "\n>"
 **/
void menu_send_n_st(void);

/**
 * @def menu_send_out_of_range
 * @brief canned message "ERROR: out of range"
 **/
void menu_send_out_of_range(void);

/**
 * @def menu_send_GSM
 * @brief canned message, sends "GSM>:"
 **/
void menu_send_GSM(void);

/**
 * @def menu_send_CAN
 * @brief canned message, sends "CAN>:"
 **/
void menu_send_CAN(void);

/**
 * @def menu_send_BTN
 * @brief canned message, sends "BTN>:"
 **/
void menu_send_BTN(void);

/**
 * @def menu_send_CSC
 * @brief canned message, sends "CSC>:"
 **/
void menu_send_CSC(void);

/**
 * @def menu_send_CTL
 * @brief canned message, sends "CTL>:"
 **/
void menu_send_CTL(void);

/**
 * @def menu_print_int
 * @brief coverts an int to char then sends over ACL usb
 * @param value the number to send over USB
 **/
void menu_print_int(long value);

/**
 * @def menu_sample_number
 * @brief converts a string starting with anumber into a long
 * @param input pointer to a string starting with - or number
 * @retval the number sent in char form
 **/
long menu_sample_number(char * input);

/**
 * @def menu_status
 * @brief Reports all systems status (cat of all others)
 **/
void menu_status (void);

/**
 * @def menu_accel
 * @brief Accelerometer menu content and conditional branching
 **/
void menu_accel (void);

/**
 * @def menu_accel_status
 * @brief  Reports accel system status
 **/
void menu_accel_status(void);

/**
 * @def menu_modem
 * @brief  modem menu content and conditional branching
 **/
void menu_modem (void);

/**
 * @def menu_modem_status
 * @brief Reports modem system status
 **/
void menu_modem_status (void);

/**
 * @def menu_can
 * @brief CANbus menu content and conditional branching
 **/
void menu_can (void);

/**
 * @def menu_can_status
 * @brief Reports canbus system status
 **/
void menu_can_status (void);

/**
 * @def menu_power
 * @brief power menu content and conditional branching
 **/
void menu_power (void);

/**
 * @def menu_power_status
 * @brief Reports power system status
 **/
void menu_power_status (void);

/**
 * @def menu_timer
 * @brief timer menu content and conditional branching
 **/
void menu_timer(void);

/**
 * @def menu_timer_status
 * @brief Reports clock system status
 **/
void menu_timer_status(void);

/**
 * @def menu_sleep
 * @brief sleep-state timer checks menu content and conditional branching
 **/
void menu_sleep(void);

/**
 * @def usb_tx_string_P
 * @brief special routine to send program memory stored strings over USB
 * @param data pointer to PSTR() stored strings
 **/
void usb_tx_string_P(const char *data);

/**
 * @def usb_tx_string_PV
 * @brief if not verbose, strings are replaced by "OK"
 * @param data pointer to PSTR() stored strings
 **/
void usb_tx_string_PV(const char *data);

/**
 * @def usb_tx_string_PVF
 * @brief if not verbose, strings are replaced by "FAIL"
 * @param data pointer to PSTR() stored strings
 **/
void usb_tx_string_PVF(const char *data);

/**
 * @def usb_tx_string_PVO
 * @brief if not verbose, nothing is sent
 * @param data pointer to PSTR() stored strings
 **/
void usb_tx_string_PVO(const char *data);

/**
 * @def menu_lockup
 * @brief special routine to dump the CMD buffer from the ACL USB interface
 **/
void menu_lockup (void);

/**
 * @def menu_verbose
 * @brief returns state of verbose variable
 **/
Bool menu_verbose(void);

#endif /* V2X_MENU_H_ */
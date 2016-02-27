/*
 * V2X_menu.h
 *
 * Created: 2/23/2016 9:00:30 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_MENU_H_
#define V2X_MENU_H_

void menu_add_to_command(char value);
void menu_main(void);
void menu_send_ok (void);
void menu_send_q (void);
void menu_send_1(void);
void menu_send_0(void);
void menu_send_n(void);
void menu_status (void);
void menu_accel (void);
void menu_accel_status(void);
void menu_modem (void);
void menu_modem_status (void);
void menu_can (void);
void menu_can_status (void);
void menu_power (void);
void menu_power_status (void);
void usb_tx_string_P(const char *data);
void clear_CMD_buffer(char * CMD_buffer);

#endif /* V2X_MENU_H_ */
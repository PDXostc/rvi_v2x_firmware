/*
 * V2X_menu.h
 *
 * Created: 2/23/2016 9:00:30 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_MENU_H_
#define V2X_MENU_H_

void menu_add_to_command(char value);
void menu_parse_command(void);
void menu_send_ok (void);
void menu_send_q (void);
void menu_send_1(void);
void menu_send_0(void);
void menu_accel (void);
void menu_modem (void);
void menu_can (void);
void menu_power (void);

#endif /* V2X_MENU_H_ */
/*
 * V2X_USB.c
 *
 * Created: 2/10/2016 2:53:56 PM
 *  Author: jbanks2
 */ 

#include "V2X.h"

void usb_start(void){
	udc_start();
}

void usb_activate_check(){
	if (host_is_on()) {udc_attach();} //atmel knows when host is active, it controls the power to host
	delay_ms(100);
	UDI_CDC_ENABLE_EXT();	
}
static bool my_flag_autorize_cdc_transfert = false;

bool my_callback_cdc_enable(void)
{
	my_flag_autorize_cdc_transfert = true;
	return true;
}
void my_callback_cdc_disable(void)
{
	my_flag_autorize_cdc_transfert = false;
}
void task(void)
{
	if (my_flag_autorize_cdc_transfert) {
		udi_cdc_putc('A');
		udi_cdc_getc();
	}
}
// 
// // Waits and gets a value on CDC line
// int udi_cdc_getc(void);
// // Reads a RAM buffer on CDC line
// iram_size_t udi_cdc_read_buf(int* buf, iram_size_t size);
// // Puts a byte on CDC line
// int udi_cdc_putc(int value);
// // Writes a RAM buffer on CDC line
// iram_size_t udi_cdc_write_buf(const int* buf, iram_size_t size);
/*
 * V2X_gsm.h
 *
 * Created: 2/12/2016 11:01:18 AM
 *  Author: jbanks2
 */ 


#ifndef V2X_GSM_H_
#define V2X_GSM_H_

/**
 * @def simcard_status
 * @brief detection of SIMCARD by socket switch 
 * returns "true" if SIMCARD installed 
 **/
 inline static bool simcard_status(void) {return 1-ioport_get_pin_level(SW1_PIN);}

#endif /* V2X_GSM_H_ */
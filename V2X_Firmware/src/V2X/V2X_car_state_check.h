/*
 * V2X_car_state_check.h
 *
 * Created: 8/9/2017 2:13:43 PM
 *  Author: dev-box
 */ 


#ifndef V2X_CAR_STATE_CHECK_H_
#define V2X_CAR_STATE_CHECK_H_



/**
	This function must:
	Change power state to enable CAN device
	Configure can device
	Take measurement of battery voltage
	Decide what to do:

	- less than 11V do no reschedule checking job
	- less than 13V and more than 11V reschedule checking job
	- more than 13V needs follow up
		-- configure can setup
		-- check for RPM
		-- start Rpi bring up sequence if greater than 0
 */
void PWR_car_on_state_check();



#endif /* V2X_CAR_STATE_CHECK_H_ */
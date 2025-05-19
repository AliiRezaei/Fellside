/*
 * main.c
 *
 *  Created on: May 17, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

int main(void)
{
	// system clock configuration
	rcc_SysClock_Config();

	// systick timer configuration
	rcc_SysTick_Config();

	// timer 3 PWM configuration
	tim_TIM3_3PWM_Config();

	// timer 2 Delay configuration
	tim_TIM2_Delay_Config();

	// BLDC motor and driver instance
	BLDCMotor_s motor;
	BLDCDriver_s driver;

	// init driver
	BLDCDriver_Init(&driver);

	// link motor and driver
	BLDCMotor_LinkDriver(&motor, &driver);

	// init motor
	int pole_pairs = 14;        // number of pole pairs
	float resistance = NOT_SET; // phase resistance
	float kv = NOT_SET;         // back emf constant
	float inductance = NOT_SET; // phase inductance
	BLDCMotor_Init(&motor, pole_pairs, resistance, inductance, kv);

	// target to go
	float target = 0.0;
	float target_arr[] = {1.0, 2.0, 3.0, 4.0};
	int i = 0;
	uint32_t start = _tim_msec_val_();
	while(1)
	{
		target = target_arr[i++];
		while (_tim_msec_val_() - start < 1500)
		{
			// run open loop
			BLDCMotor_Move(&motor, target);
		}
		if (i > 3)
			i = 0;
		start = _tim_msec_val_();
	}
//	// target to go
//	float target = -1.0;
//	while (1)
//	{
//		BLDCMotor_Move(&motor, target);
//	}
}


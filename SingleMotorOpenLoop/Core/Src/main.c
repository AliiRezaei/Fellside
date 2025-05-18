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
	float kv = NOT_SET; // back emf constant
	float inductance = NOT_SET; // phase inductance
	BLDCMotor_Init(&motor, pole_pairs, resistance, inductance, kv);

	// target to go
	float target = _PI;

	while(1)
	{
		// run open loop
		BLDCMotor_Move(&motor, target);
	}
}


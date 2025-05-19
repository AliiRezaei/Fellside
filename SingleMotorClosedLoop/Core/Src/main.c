/*
 * main.c
 *
 *  Created on: May 18, 2025
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

	// i2c magnetic sensor configuration
	i2c_I2C1_Config();

	// as5600 sensor instance
	Sensor_s sensor;

	// init sensor
	sensor_Init(&sensor);

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

	// velocity PID
	float PID_velocity_P = 1.5f;
	float PID_velocity_I = 2.5f;
	float PID_velocity_D = 0.0f;
	pid_SetGains(&(motor.PID_velocity), PID_velocity_P, PID_velocity_I,
			PID_velocity_D, DEF_PID_VEL_RAMP, DEF_PID_VEL_LIM);

	// angle PID
	float PID_angle_P = 40.0f;
	float PID_angle_I = 0.5f;
	float PID_angle_D = 0.0f;
	pid_SetGains(&(motor.P_angle), PID_angle_P, PID_angle_I,
			PID_angle_D, DEF_PID_ANG_RAMP, DEF_PID_ANG_LIM);

	// velocity and angle Low-Pass filters time constants
	lpf_SetTimeConstant(&(motor.LPF_velocity), 0.005f);
	lpf_SetTimeConstant(&(motor.LPF_angle), 0.005f);

//	// init field oriented control
//	BLDCMotor_InitFOC(&motor);

	// target to go
	float target = 0.0;

	while(1)
	{
		// run closed loop
		BLDCMotor_Move(&motor, target);
	}
}


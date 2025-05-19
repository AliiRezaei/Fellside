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

	// let test as5600
	// as5600 sensor instance
	Sensor_s sensor;

	// init sensor
	sensor_Init(&sensor);

	while(1)
	{
		sensor_Update(&sensor);
		tim_TIM2_Delay_ms(100);
	}
}


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

	// test PWMs
	TIM3->CCR1 = 1680 * 0.25;
	TIM3->CCR2 = 1680 * 0.50;
	TIM3->CCR3 = 1680 * 0.75;

	while(1) {;}
}


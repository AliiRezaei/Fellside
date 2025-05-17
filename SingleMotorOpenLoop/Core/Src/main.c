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

	// enable blue led for debugging
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODE15_0;
	GPIOD->MODER &= ~GPIO_MODER_MODE15_1;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;

	while(1)
	{
		// test delay
		GPIOD->ODR |=  GPIO_ODR_OD15;
		tim_TIM2_Delay_us(1000);
		GPIOD->ODR &= ~GPIO_ODR_OD15;
		tim_TIM2_Delay_us(1000);
	}
}


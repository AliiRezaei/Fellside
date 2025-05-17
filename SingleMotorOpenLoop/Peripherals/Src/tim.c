/*
 * tim.c
 *
 *  Created on: May 17, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief : Timer3 Configuration for Generating 3 PWMs in PA7, PA6, PB0
 * @note  :
 *          Center aligned mode 3
 *          Frequency 25 KHz
 *          PWM mode 1
 */
void tim_TIM3_3PWM_Config(void)
{
	// enable clocks for GPIOA and GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

	// set PA6, PA7 to alternate function (TIM3_CH1, TIM3_CH2) AF2
	GPIOA->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
	GPIOA->MODER |=  (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1); // Alternate Function

	GPIOA->AFR[0] &= ~((0xF << (6 * 4)) | (0xF << (7 * 4)));
	GPIOA->AFR[0] |=  ((0x2 << (6 * 4)) | (0x2 << (7 * 4))); // AF2 for TIM3

	// set PB0 to alternate function (TIM3_CH3) AF2
	GPIOB->MODER &= ~(GPIO_MODER_MODER0);
	GPIOB->MODER |=  (GPIO_MODER_MODER0_1); // Alternate Function

	GPIOB->AFR[0] &= ~(0xF << (0 * 4));
	GPIOB->AFR[0] |=  (0x2 << (0 * 4)); // AF2 for TIM3

	// set speed, push-pull
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;

	// enable TIM3 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// center aligned mode 3
	TIM3->CR1 &= ~TIM_CR1_CMS;
	TIM3->CR1 |=  TIM_CR1_CMS_1 | TIM_CR1_CMS_0; // CMS = 11

	// set auto reload value and prescaler
	TIM3->ARR = 1680 - 1; // because of APB1 clock is 168 MHz and center aligned mode 3
	TIM3->PSC = 0;

	// configure CH1, CH2 in CCMR1 (PWM mode 1, preload enable)
	TIM3->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
	TIM3->CCMR1 |=  (6 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;
	TIM3->CCMR1 |=  (6 << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE;

	// configure CH3 in CCMR2
	TIM3->CCMR2 &= ~TIM_CCMR2_OC3M;
	TIM3->CCMR2 |= (6 << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE;

	// enable outputs
	TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;

	// enable auto reload preload
	TIM3->CR1 |= TIM_CR1_ARPE;

	// force update to load registers
	TIM3->EGR |= TIM_EGR_UG;

	// start timer
	TIM3->CR1 |= TIM_CR1_CEN;
}

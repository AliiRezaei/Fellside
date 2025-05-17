/*
 * rcc.c
 *
 *  Created on: May 17, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief : System Clock Configuration
 * @Note  :
 *          SYSCLK 168 MHz
 * 		    HCLK   168 MHz
 * 		    APB1    42 MHz
 *		    APB2    84 MHz
 */
void rcc_SysClock_Config(void)
{
	// enable HSE
	RCC->CR |= RCC_CR_HSEON;               // HSE on
	while (!(RCC->CR & RCC_CR_HSERDY)) {;} // wait for it

	// select HSE as PLL source
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE);

	// PLL prescalers
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLM); // clear previous
	RCC->PLLCFGR |=  (0 << RCC_PLLCFGR_PLLP_Pos)                               // PLLP =   2
	             |   (336 << RCC_PLLCFGR_PLLN_Pos)                             // PLLN = 336
			     |   (8 << RCC_PLLCFGR_PLLM_Pos)                               // PLLM =   8
			     |   (7 << RCC_PLLCFGR_PLLQ_Pos);                              // PLLQ =   7

	// enable main PLL
	RCC->CR |= RCC_CR_PLLON;               // main PLL on
	while (!(RCC->CR & RCC_CR_PLLRDY)) {;} // wait for it

	// APB1, APB2, AHB prescalers
	RCC->CFGR &= ~(RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1); // clear previous
	RCC->CFGR |=  (0b100 << RCC_CFGR_PPRE2_Pos)      // APB2 prescalers = 2
	          |   (0b101 << RCC_CFGR_PPRE1_Pos);     // APB1 prescalers = 4
	RCC->CFGR &= ~ RCC_CFGR_HPRE;                    // AHB not divided

	// proper flash latency for SYSCLK = 168 MHz
	FLASH->ACR &= ~ FLASH_ACR_LATENCY;     // clear previous
	FLASH->ACR |=   FLASH_ACR_LATENCY_5WS; // 5 wait states for 168 MHz

	// PLL selected as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while (!(RCC->CFGR & RCC_CFGR_SWS)) {;}  // wait for it
}

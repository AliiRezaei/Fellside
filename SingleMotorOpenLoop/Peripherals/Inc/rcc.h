/*
 * rcc.h
 *
 *  Created on: May 17, 2025
 *      Author: Ali Rezaei
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_

/*
 * @brief : System Clock Configuration
 * @Note  :
 *          SYSCLK 168 MHz
 * 		    HCLK   168 MHz
 * 		    APB1    42 MHz
 *		    APB2    84 MHz
 */
void rcc_SysClock_Config(void);

/*
 * @brief : SysTick Timer Configuration
 * @Note  : adjusted in 1ms regarding 168 MHz system clock
 */
void rcc_SysTick_Config(void);

#endif /* INC_RCC_H_ */

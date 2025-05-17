/*
 * tim.h
 *
 *  Created on: May 17, 2025
 *      Author: Ali Rezaei
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

/*
 * @brief : Timer3 Configuration for Generating 3 PWMs in PA7, PA6, PB0
 * @note  :
 *          Center aligned mode 3
 *          Frequency 25 KHz
 *          PWM mode 1
 */
void tim_TIM3_3PWM_Config(void);

/*
 * @brief : Timer 2 Configuration for Delay Generating
 * @note  :
 *          milliseconds and microseconds are available
 *          so, timer counting-up and overflow each 1ms
 */
void tim_TIM2_Delay_Config(void);

/*
 * @brief  : Get milliseconds Value at this Moment
 * @retval : milliseconds value
 */
uint32_t _tim_msec_val_(void);

/*
 * @brief  : Get microseconds Value at this Moment
 * @retval : microseconds value
 */
uint32_t _tim_usec_val_(void);

/*
 * @brief : Timer2 Delay milliseconds
 * @param : milliseconds delay
 */
void tim_TIM2_Delay_ms(uint32_t ms);

/*
 * @brief : Timer2 Delay microseconds
 * @param : microseconds delay
 */
void tim_TIM2_Delay_us(uint32_t us);

#endif /* INC_TIM_H_ */

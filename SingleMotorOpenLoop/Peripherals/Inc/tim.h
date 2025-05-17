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

#endif /* INC_TIM_H_ */

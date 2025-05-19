/*
 * lpf.h
 *
 *  Created on: Apr 24, 2025
 *      Author: Ali Rezaei
 */

#ifndef LOWPASSFILTER_INC_LPF_H_
#define LOWPASSFILTER_INC_LPF_H_

/*
 * @brief Discrete Low-pass Filter Structure
*/
typedef struct
{
	float Tf;              // filter time constant
	unsigned long Ts_prev; // previous step sample time
	float y_prev;          // filtered value in previous execution step
} LowPassFilter_s;

/*
 * @brief Low-Pass Filter Initialization
*/
void lpf_Init(LowPassFilter_s *lpf);

/*
 * @brief Low-Pass Filter Set Time Constant
*/
void lpf_SetTimeConstant(LowPassFilter_s *lpf, float Tf);

/*
 * @brief Low-Pass Filter Operator
*/
float lpf_Operator(LowPassFilter_s *lpf, float y);

/*
 * @brief Low-Pass Filter Reset
*/
void lpf_Reset(LowPassFilter_s *lpf);



#endif /* LOWPASSFILTER_INC_LPF_H_ */

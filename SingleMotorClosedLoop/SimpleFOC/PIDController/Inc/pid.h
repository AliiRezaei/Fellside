/*
 * pid.h
 *
 *  Created on: Apr 23, 2025
 *      Author: Ali Rezaei
 */

#ifndef PIDCONTROLLER_INC_PID_H_
#define PIDCONTROLLER_INC_PID_H_

/*
 * @brief PID Controller Structure
*/
typedef struct
{
	float P;         // P gain
	float I;         // I gain
	float D;         // D gain
	float ramp;      // maximum speed of change of the output value
	float sat;       // saturation (maximum allowable output value)
	float e_prev;    // previous error value
	float integral_prev;  // integral  of error
	float output_prev;  // output
	unsigned long timestamp_prev;
} PIDController_s;

/*
 * @brief PID Controller Init
*/
void pid_Init(PIDController_s *pid);

/*
 * @brief PID Controller Set Gains
*/
void pid_SetGains(PIDController_s *pid, float p, float i, float d, float ramp, float sat);

/*
 * @brief PID Controller Operator
*/
float pid_Operator(PIDController_s *pid, float e);

/*
 * @brief PID Controller Reset
*/
void pid_Reset(PIDController_s *pid);


#endif /* PIDCONTROLLER_INC_PID_H_ */

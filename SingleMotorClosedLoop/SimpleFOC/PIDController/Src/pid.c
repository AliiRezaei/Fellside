/*
 * pid.c
 *
 *  Created on: Apr 23, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief PID Controller Init
 *
*/

void pid_Init(PIDController_s *pid)
 {
	pid_Reset(pid);
	pid->timestamp_prev = _tim_usec_val_();
}

/*
 * @brief PID Controller Set Gains
 *
*/

void pid_SetGains(PIDController_s *pid, float p, float i, float d, float ramp, float sat)
{
	pid->P    = p;
	pid->I    = i;
	pid->D    = d;
	pid->ramp = ramp;
	pid->sat  = sat;
}

/*

 * @brief PID Controller Operator
 * @param pid - pid controller structure
 * @param e   - error at this moment
*/

float pid_Operator(PIDController_s *pid, float e)
{
	// pid gains
	float pGain = pid->P;
	float iGain = pid->I;
	float dGain = pid->D;

	// calculate the time from the last call
	unsigned long timestamp_now = _tim_usec_val_();
	float Ts = (timestamp_now - pid->timestamp_prev) * 1e-6f;
	// quick fix for strange cases (micros overflow)
	if (Ts <= 0 || Ts > 0.5f)
	{
		Ts = 1e-3f;
	}

	// pid terms
	float pTerm = pGain * e;
	float iTerm = pid->integral_prev + iGain*Ts*0.5f*(e + pid->e_prev);
	float dTerm = dGain*(e - pid->e_prev)/Ts;

	// saturated integral term
	iTerm = _constrain(iTerm, - pid->sat, pid->sat);

	// control signal
	float output = pTerm + iTerm + dTerm;
	output = _constrain(output, - pid->sat, pid->sat);

	// if output ramp defined
	if (pid->ramp > 0)
	{
		// limit the acceleration by ramping the output
		float output_rate = (output - pid->output_prev) / Ts;
		if (output_rate > pid->ramp)
			output = pid->output_prev + pid->ramp * Ts;
		else if (output_rate < - pid->ramp)
			output = pid->output_prev - pid->ramp * Ts;
	}

	// update pid
	pid->integral_prev = iTerm;
	pid->output_prev = output;
	pid->e_prev = e;
	pid->timestamp_prev = timestamp_now;

	// return output
	return output;
}

/*
 * @brief PID Controller Reset
*/

void pid_Reset(PIDController_s *pid)
 {
	pid->e_prev   = 0.0f;
	pid->integral_prev = 0.0f;
	pid->output_prev = 0.0f;
}


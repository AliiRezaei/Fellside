/*
 * lpf.c
 *
 *  Created on: Apr 24, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief Low-Pass Filter Initialization
*/
void lpf_Init(LowPassFilter_s *lpf)
{
	lpf->y_prev  = 0.0f;
	lpf->Ts_prev = _tim_usec_val_();
}

/*
 * @brief Low-Pass Filter Set Time Constant
 */
void lpf_SetTimeConstant(LowPassFilter_s *lpf, float Tf)
{
	lpf->Tf = Tf;
}

/*
 * @brief Low-Pass Filter Operator
 */
float lpf_Operator(LowPassFilter_s *lpf, float y)
 {
	unsigned long Ts = _tim_usec_val_();
	float dt = (Ts - lpf->Ts_prev) * 1e-6f;

	if (dt < 0.0f)
		dt = 1e-3f;
	else if (dt > 0.3f) {
		lpf->y_prev = y;
		lpf->Ts_prev = Ts;
		return y;
	}

	float alpha = lpf->Tf / (lpf->Tf + dt);
	float y_filterd = alpha * lpf->y_prev + (1.0f - alpha) * y;
	lpf->y_prev  = y_filterd;
	lpf->Ts_prev = Ts;
	return y_filterd;
}

/*
 * @brief Low-Pass Filter Reset
 */
void lpf_Reset(LowPassFilter_s *lpf)
{
	lpf->y_prev  = 0.0f;
	lpf->Ts_prev = _tim_usec_val_();
}

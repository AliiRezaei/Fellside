/*
 * BLDCMotor.c
 *
 *  Created on: May 18, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief : BLDC Motor Move to Target
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s structure, handle motor params
 * 			target    --> target angle in radians
 */
void BLDCMotor_Move(BLDCMotor_s *BLDCMotor, float target)
{
	// set internal target variable
	BLDCMotor->target = target;

	// calculate the back-emf voltage if KV_rating available U_bemf = vel*(1/KV)
	if (_isset(BLDCMotor->KV_rating))
		BLDCMotor->voltage_bemf = BLDCMotor->shaft_velocity
				/ (BLDCMotor->KV_rating * _SQRT3) / _RPM_TO_RADS;

	// estimate the motor current if phase resistance available
	if (_isset(BLDCMotor->phase_resistance))
		BLDCMotor->current.q = (BLDCMotor->voltage.q
				- BLDCMotor->voltage_bemf)
				/ BLDCMotor->phase_resistance;

	// set target
	BLDCMotor->shaft_angle_sp = BLDCMotor->target;
	BLDCMotor_RunOpenloop(BLDCMotor, BLDCMotor->shaft_angle_sp);
}

/*
 * @brief : BLDC Motor Run OpenLoop, Caller is BLDCMotor_Move
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s structure, handle motor params
 * 			target    --> target angle in radians
 */
void BLDCMotor_RunOpenloop(BLDCMotor_s *BLDCMotor, float target)
{
	// get current time
	unsigned long now_us = _tim_usec_val_();

	// calculate the sample time from last call
	float Ts = (now_us - BLDCMotor->open_loop_timestamp) * 1e-6f;

	// quick fix for strange cases (timer overflow or time stamp not defined)
	if (Ts <= 0 || Ts > 0.5f)
		Ts = 1e-3f;

	// calculate the necessary angle to move from current position towards target angle
	// with maximal velocity (velocity_limit)
	if (fabs(target - BLDCMotor->shaft_angle)
			> fabs(BLDCMotor->velocity_limit * Ts))
	{
		BLDCMotor->shaft_angle +=
				_sign(target - BLDCMotor->shaft_angle)
						* fabs(BLDCMotor->velocity_limit) * Ts;
		BLDCMotor->shaft_velocity = BLDCMotor->velocity_limit;
	}
	else
	{
		BLDCMotor->shaft_angle = target;
		BLDCMotor->shaft_velocity = 0;
	}

	// use voltage limit or current limit
	float Uq = BLDCMotor->voltage_limit;
	if (_isset(BLDCMotor->phase_resistance))
	{
		Uq = _constrain(
				BLDCMotor->current_limit
						* BLDCMotor->phase_resistance
						+ fabs(BLDCMotor->voltage_bemf),
				-BLDCMotor->voltage_limit,
				BLDCMotor->voltage_limit);
		// recalculate the current
		BLDCMotor->current.q = (Uq
				- fabs(BLDCMotor->voltage_bemf))
				/ BLDCMotor->phase_resistance;
	}
	// set the maximal allowed voltage (voltage_limit) with the necessary angle
	float elec_angle = BLDCMotor->shaft_angle * BLDCMotor->pole_pairs;
	BLDCMotor_SetPhaseVoltage(BLDCMotor, Uq, 0, elec_angle);

	// save time stamp for next call
	BLDCMotor->open_loop_timestamp = now_us;
}

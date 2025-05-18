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
 * 			_PP       --> pole pairs
 * 			_R        --> phase resistance (Not Set by default)
 * 			_L        --> phase inductance (Not Set by default)
 * 			_KV       --> kv rating        (Not Set by default)
 */
void BLDCMotor_Init(BLDCMotor_s *BLDCMotor, int _PP,  float _R, float _KV, float _L)
{
	// assume modulation centered
	BLDCMotor->feed_forward_velocity = 0.0f;
	BLDCMotor->modulation_centered = 1;

	// maximum velocity to be set to the motor
	BLDCMotor->velocity_limit = DEF_MOT_VEL_LIM;

	// maximum voltage to be set to the motor
	BLDCMotor->voltage_limit  = DEF_MOT_VOLT_LIM;

	// maximum current to be set to the motor
	BLDCMotor->current_limit  = DEF_MOT_CURR_LIM;

	// default target value
	BLDCMotor->target    = 0;
	BLDCMotor->current.d = 0;
	BLDCMotor->current.q = 0;
	BLDCMotor->voltage.d = 0;
	BLDCMotor->voltage.q = 0;

	// voltage back emf
	BLDCMotor->voltage_bemf = 0;

	// initial phase voltages
	BLDCMotor->Ua = 0.0;
	BLDCMotor->Ub = 0.0;
	BLDCMotor->Uc = 0.0;

	// initial Park and Clarke voltages
	BLDCMotor->Ualpha = 0.0;
	BLDCMotor->Ubeta  = 0.0;

	// save pole pairs number
	BLDCMotor->pole_pairs = _PP;

	// save phase resistance number
	BLDCMotor->phase_resistance = NOT_SET;
	if (_isset(_R))
		BLDCMotor->phase_resistance = _R;

	// save back emf constant KV = 1/KV
	// 1/sqrt(2) - rms value
	BLDCMotor->KV_rating = NOT_SET;
	if (_isset(_KV))
		BLDCMotor->KV_rating = _KV;

	// save phase inductance
	BLDCMotor->phase_inductance = NOT_SET;
	if (_isset(_L))
		BLDCMotor->phase_inductance = _L;

	// sanity check for the voltage limit configuration
	if (BLDCMotor->voltage_limit
			> BLDCMotor->BLDCDriver->voltage_limit)
		BLDCMotor->voltage_limit =
				BLDCMotor->BLDCDriver->voltage_limit;

	// initial angles
	BLDCMotor->shaft_angle      = 0.0;
	BLDCMotor->electrical_angle = 0.0;

	// set zero to PWM
	BLDCDriver_SetPWM(BLDCMotor->BLDCDriver, 0.0f, 0.0f, 0.0f);
}

/*
 * @brief : BLDC Motor and Driver Linker
 * @param :
 * 			BLDCMotor  --> pointer to BLDCMotor_s  structure, handle motor  params
 * 			BLDCDriver --> pointer to BLDCDriver_s structure, handle driver params
 */
void BLDCMotor_LinkDriver(BLDCMotor_s *BLDCMotor, BLDCDriver_s *BLDCDriver)
{
	// link driver
	BLDCMotor->BLDCDriver = BLDCDriver;
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
 * @brief : BLDC Motor Set Phase Voltage, leverages Park and Clarke (inv)transforms
 *          to calculate and set the phase voltages
 * @param :
 * 			BLDCMotor  --> pointer to BLDCMotor_s  structure, handle motor  params
 * 			Uq, Ud     --> Park and Clarke voltages
 * 			elec_angle --> electrical angle
 */
void BLDCMotor_SetPhaseVoltage(BLDCMotor_s *BLDCMotor, float Uq, float Ud, float elec_angle)
{
	float center;
	float _ca, _sa;

	// sinusoidal PWM modulation Inverse Park + Clarke transformation
	_sincos(elec_angle, &_sa, &_ca);

	// Inverse park transform
	BLDCMotor->Ualpha = _ca * Ud - _sa * Uq;  // -sin(angle) * Uq;
	BLDCMotor->Ubeta  = _sa * Ud + _ca * Uq;  //  cos(angle) * Uq;

	// Clarke transform
	BLDCMotor->Ua = BLDCMotor->Ualpha;
	BLDCMotor->Ub = -0.5f * BLDCMotor->Ualpha + _SQRT3_2 * BLDCMotor->Ubeta;
	BLDCMotor->Uc = -0.5f * BLDCMotor->Ualpha - _SQRT3_2 * BLDCMotor->Ubeta;

	center = BLDCMotor->voltage_limit / 2;

	// Midpoint Clamp
	float Umin = fmin(BLDCMotor->Ua, fmin(BLDCMotor->Ub, BLDCMotor->Uc));
	float Umax = fmax(BLDCMotor->Ua, fmax(BLDCMotor->Ub, BLDCMotor->Uc));
	center -= (Umax + Umin) / 2;

	if (!BLDCMotor->modulation_centered)
	{
		BLDCMotor->Ua -= Umin;
		BLDCMotor->Ub -= Umin;
		BLDCMotor->Uc -= Umin;
	}
	else
	{
		BLDCMotor->Ua += center;
		BLDCMotor->Ub += center;
		BLDCMotor->Uc += center;
	}
	// set the voltages in driver
	BLDCDriver_SetPWM(BLDCMotor->BLDCDriver, BLDCMotor->Ua, BLDCMotor->Ub, BLDCMotor->Uc);
}

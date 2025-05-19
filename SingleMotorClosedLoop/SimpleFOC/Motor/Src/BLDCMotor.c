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

	// initial zero electric angle
	BLDCMotor->zero_electric_angle = 0.0;

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
	if (BLDCMotor->voltage_limit > BLDCMotor->BLDCDriver->voltage_limit)
		BLDCMotor->voltage_limit = BLDCMotor->BLDCDriver->voltage_limit;

	// init current_q pid controller
	pid_Init(&(BLDCMotor->PID_current_q));
	pid_SetGains(&(BLDCMotor->PID_current_q), DEF_PID_CURR_P, DEF_PID_CURR_I,
			DEF_PID_CURR_D, DEF_PID_CURR_RAMP, DEF_POWER_SUPPLY);

	// init current_d pid controller
	pid_Init(&(BLDCMotor->PID_current_d));
	pid_SetGains(&(BLDCMotor->PID_current_d), DEF_PID_CURR_P, DEF_PID_CURR_I,
			DEF_PID_CURR_D, DEF_PID_CURR_RAMP, DEF_POWER_SUPPLY);

	// init velocity pid controller
	pid_Init(&(BLDCMotor->PID_velocity));
	pid_SetGains(&(BLDCMotor->PID_velocity), DEF_PID_VEL_P, DEF_PID_VEL_I,
	DEF_PID_VEL_D, DEF_PID_VEL_RAMP, DEF_PID_VEL_LIM);

	// init angle pid controller
	pid_Init(&(BLDCMotor->P_angle));
	pid_SetGains(&(BLDCMotor->P_angle), DEF_PID_ANG_P, DEF_PID_ANG_I,
			DEF_PID_ANG_D,
			DEF_PID_ANG_RAMP, DEF_PID_ANG_LIM);

	// init low-pass filter current_q
	lpf_Init(&(BLDCMotor->LPF_current_q));
	lpf_SetTimeConstant(&(BLDCMotor->LPF_current_q), DEF_CURR_FILTER_Tf);

	// init low-pass filter current_d
	lpf_Init(&(BLDCMotor->LPF_current_d));
	lpf_SetTimeConstant(&(BLDCMotor->LPF_current_d), DEF_CURR_FILTER_Tf);

	// init low-pass filter velocity
	lpf_Init(&(BLDCMotor->LPF_velocity));
	lpf_SetTimeConstant(&(BLDCMotor->LPF_velocity), DEF_VEL_FILTER_Tf);

	// init low-pass filter angle
	lpf_Init(&(BLDCMotor->LPF_angle));
	lpf_SetTimeConstant(&(BLDCMotor->LPF_angle), DEF_ANG_FILTER_Tf);

	// maximum angular velocity to be used for positioning
	BLDCMotor->velocity_limit = DEF_MOT_VEL_LIM;
	// maximum voltage to be set to the motor
	BLDCMotor->voltage_limit = DEF_MOT_VOLT_LIM;
	// not set on the begining
	BLDCMotor->current_limit = DEF_CURRENT_LIM;

	// index search velocity
	BLDCMotor->velocity_index_search = DEF_INDEX_SEARCH_TARGET_VELOCITY;
	// sensor and motor align voltage
	BLDCMotor->voltage_sensor_align = DEF_VOLTAGE_SENSOR_ALIGN;

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
 * @brief : BLDC Motor and Sensor Linker
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s  structure, handle motor  params
 * 			Sensor    --> pointer to Sensor_s structure, handle sensor params
 */
void BLDCMotor_LinkSensor(BLDCMotor_s *BLDCMotor, Sensor_s *Sensor)
{
	// link sensor
	BLDCMotor->Sensor = Sensor;
}

/*
 * @brief : BLDC Motor Run OpenLoop, Caller is BLDCMotor_Move
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s structure, handle motor params
 * 			target    --> target angle in radians
 */
void BLDCMotor_RunClosedLoop(BLDCMotor_s *BLDCMotor, float target)
{
	// update sensor
	sensor_Update(BLDCMotor->Sensor);

	// electrical angle
	BLDCMotor->electrical_angle = sensor_GetElecAngle(BLDCMotor->Sensor,
			BLDCMotor->zero_electric_angle, BLDCMotor->pole_pairs);

	// angle set point
	BLDCMotor->shaft_angle = sensor_ShaftAngle(&(BLDCMotor->LPF_angle), BLDCMotor->Sensor);
	BLDCMotor->shaft_angle_sp = target;

	// calculate velocity set point
	BLDCMotor->shaft_velocity = sensor_ShaftVelocity(&(BLDCMotor->LPF_velocity),
			BLDCMotor->Sensor);
	BLDCMotor->shaft_velocity_sp = BLDCMotor->feed_forward_velocity
			+ pid_Operator(&(BLDCMotor->P_angle),
					BLDCMotor->shaft_angle_sp - BLDCMotor->shaft_angle);
	BLDCMotor->shaft_velocity_sp = _constrain(BLDCMotor->shaft_velocity_sp,
			-BLDCMotor->velocity_limit, BLDCMotor->velocity_limit);

	// calculate the current set point
	BLDCMotor->current_sp = pid_Operator(&(BLDCMotor->PID_velocity),
			BLDCMotor->shaft_velocity_sp - BLDCMotor->shaft_velocity);

	// use voltage if phase-resistance not provided
	if (!_isset(BLDCMotor->phase_resistance))
		BLDCMotor->voltage.q = BLDCMotor->current_sp;
	else
		BLDCMotor->voltage.q = _constrain(
				BLDCMotor->current_sp * BLDCMotor->phase_resistance
						+ BLDCMotor->voltage_bemf, -BLDCMotor->voltage_limit,
				BLDCMotor->voltage_limit);
	// set d-component (lag compensation if known inductance)
	if (!_isset(BLDCMotor->phase_inductance))
		BLDCMotor->voltage.d = 0;
	else
		BLDCMotor->voltage.d = _constrain(
				-BLDCMotor->current_sp * BLDCMotor->shaft_velocity
						* BLDCMotor->pole_pairs * BLDCMotor->phase_inductance,
				-BLDCMotor->voltage_limit, BLDCMotor->voltage_limit);

	// set the phases voltage
	BLDCMotor_SetPhaseVoltage(BLDCMotor, BLDCMotor->voltage.q,
			BLDCMotor->voltage.d, BLDCMotor->electrical_angle);
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
	BLDCMotor->target = fabs(target);

	// movement direction
	BLDCMotor->direction = _sign(target);

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
	BLDCMotor_RunClosedLoop(BLDCMotor, BLDCMotor->shaft_angle_sp);
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

	// swap PWM pins regarding movement direction
	if (BLDCMotor->direction < 0)
	{
		float temp = BLDCMotor->Ub;
		BLDCMotor->Ub = BLDCMotor->Uc;
		BLDCMotor->Uc = temp;
	}

	// set the voltages in driver
	BLDCDriver_SetPWM(BLDCMotor->BLDCDriver, BLDCMotor->Ua, BLDCMotor->Ub, BLDCMotor->Uc);
}

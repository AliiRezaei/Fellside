/*
 * BLDCMotor.h
 *
 *  Created on: May 18, 2025
 *      Author: Ali Rezaei
 */

#ifndef MOTOR_INC_BLDCMOTOR_H_
#define MOTOR_INC_BLDCMOTOR_H_

/*
 * @brief : DQ Voltage Structure
*/
typedef struct
{
    float d;
    float q;
} DQVoltage_s;

/*
 * @brief : DQ Current Structure
*/
typedef struct
{
    float d;
    float q;
} DQCurrent_s;

/*
 * @brief : BLDC Motor Structure
 */
typedef struct
{
	// state variables
	float target;                // target value - depends of the controller
	float feed_forward_velocity; // feed forward velocity
	float shaft_angle;           // motor angle
	float electrical_angle;      // electrical angle
	float zero_electric_angle;   // absolute zero electric angle
	float shaft_velocity;        // motor velocity
	float shaft_velocity_sp;     // target velocity
	float shaft_angle_sp;        // target angle
	float current_sp;            // current set point
	DQVoltage_s voltage;         // d and q voltage set to the motor
	DQCurrent_s current;         // d and q current measured
	float voltage_bemf;          // estimated backemf voltage (if provided KV constant)

	// motor physical parameters
	float phase_resistance;      // motor phase resistance
	int   pole_pairs;            // motor pole pairs number
	float KV_rating;             // motor KV rating
	float phase_inductance;      // motor phase inductance

	// motor configuration parameters
	float voltage_sensor_align;  // sensor and motor align voltage parameter
	float velocity_index_search; // target velocity for index search

	// limiting variables
	float voltage_limit;         // voltage limiting variable
	float current_limit;         // current limiting variable
	float velocity_limit;        // velocity limiting variable

	// phase voltages
	float Ua, Ub, Uc;

	// Phase voltages U alpha and U beta used for inverse Park and Clarke transform
	float Ualpha, Ubeta;

	// pwm modulation related variables
	uint8_t modulation_centered; // flag (1) centered modulation around driver limit /2  or  (0) pulled to 0

	// driver instance
	BLDCDriver_s *BLDCDriver;

	// sensor instance
	Sensor_s *Sensor;

	// movement direction
	float direction;

	// controllers and low pass filters
	PIDController_s PID_current_q; // parameter determining the q current PID config
	PIDController_s PID_current_d; // parameter determining the d current PID config
	LowPassFilter_s LPF_current_q; //  parameter determining the current Low pass filter configuration
	LowPassFilter_s LPF_current_d; //  parameter determining the current Low pass filter configuration
	PIDController_s PID_velocity;  // parameter determining the velocity PID configuration
	PIDController_s PID_angle;     // parameter determining the position PID configuration
	LowPassFilter_s LPF_velocity;  //  parameter determining the velocity Low pass filter configuration
	LowPassFilter_s LPF_angle;     // parameter determining the angle low pass filter configuration
} BLDCMotor_s;

/*
 * @brief : BLDC Motor Move to Target
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s structure, handle motor params
 * 			_PP       --> pole pairs
 * 			_R        --> phase resistance (Not Set by default)
 * 			_L        --> phase inductance (Not Set by default)
 * 			_KV       --> kv rating        (Not Set by default)
 */
void BLDCMotor_Init(BLDCMotor_s *BLDCMotor, int _PP,  float _R, float _KV, float _L);

/*
 * @brief : BLDC Motor and Driver Linker
 * @param :
 * 			BLDCMotor  --> pointer to BLDCMotor_s  structure, handle motor  params
 * 			BLDCDriver --> pointer to BLDCDriver_s structure, handle driver params
 */
void BLDCMotor_LinkDriver(BLDCMotor_s *BLDCMotor, BLDCDriver_s *Driver);

/*
 * @brief : BLDC Motor and Sensor Linker
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s  structure, handle motor  params
 * 			Sensor    --> pointer to Sensor_s structure, handle sensor params
 */
void BLDCMotor_LinkSensor(BLDCMotor_s *BLDCMotor, Sensor_s *Sensor);

/*
 * @brief : BLDC Motor Run ClosedLoop, Caller is BLDCMotor_Move
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s structure, handle motor params
 * 			target    --> target angle in radians
 */
void BLDCMotor_RunClosedLoop(BLDCMotor_s *BLDCMotor, float target);

/*
 * @brief : BLDC Motor Move to Target
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s structure, handle motor params
 * 			target    --> target angle in radians
 */
void BLDCMotor_Move(BLDCMotor_s *BLDCMotor, float target);

/*
 * @brief : BLDC Motor Set Phase Voltage, leverages Park and Clarke (inv)transforms
 *          to calculate and set the phase voltages
 * @param :
 * 			BLDCMotor  --> pointer to BLDCMotor_s  structure, handle motor  params
 * 			Uq, Ud     --> Park and Clarke voltages
 * 			elec_angle --> electrical angle
 */
void BLDCMotor_SetPhaseVoltage(BLDCMotor_s *BLDCMotor, float Uq, float Ud, float elec_angle);

#endif /* MOTOR_INC_BLDCMOTOR_H_ */

/*
 * BLDCMotor.h
 *
 *  Created on: May 18, 2025
 *      Author: Ali Rezaei
 */

#ifndef MOTOR_INC_BLDCMOTOR_H_
#define MOTOR_INC_BLDCMOTOR_H_

/*
 * @brief : BLDC Motor Structure
 */
typedef struct
{
	// state variables
	float target;                // current target value - depends of the controller
	float feed_forward_velocity; // current feed forward velocity
	float shaft_angle;           // current motor angle
	float electrical_angle;      // current electrical angle
	float shaft_velocity;        // current motor velocity
	float shaft_velocity_sp;     // current target velocity
	float shaft_angle_sp;        // current target angle
	DQVoltage_s voltage;         // current d and q voltage set to the motor
	float voltage_bemf;          // estimated backemf voltage (if provided KV constant)
	float Ualpha, Ubeta;         // Phase voltages U alpha and U beta used for inverse Park and Clarke transform

	// motor physical parameters
	float phase_resistance;      // motor phase resistance
	int   pole_pairs;            // motor pole pairs number
	float KV_rating;             // motor KV rating
	float phase_inductance;      // motor phase inductance

	// limiting variables
	float voltage_limit;         // voltage limiting variable - global limit
	float velocity_limit;        // velocity limiting variable - global limit

	// pwm modulation related variables
	uint8_t modulation_centered; // flag (1) centered modulation around driver limit /2  or  (0) pulled to 0
} BLDCMotor_s;

/*
 * @brief : BLDC Motor Move to Target
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s structure, handle motor params
 * 			target    --> target angle in radians
 */
void BLDCMotor_Move(BLDCMotor_s *BLDCMotor, float target);

/*
 * @brief : BLDC Motor Run OpenLoop, Caller is BLDCMotor_Move
 * @param :
 * 			BLDCMotor --> pointer to BLDCMotor_s structure, handle motor params
 * 			target    --> target angle in radians
 */
void BLDCMotor_RunOpenloop(BLDCMotor_s *BLDCMotor, float target);







#endif /* MOTOR_INC_BLDCMOTOR_H_ */

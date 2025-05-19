/*
 * sensor.h
 *
 *  Created on: Apr 24, 2025
 *      Author: Ali Rezaei
 */

#ifndef SENSOR_INC_SENSOR_H_
#define SENSOR_INC_SENSOR_H_

/*
 * @brief : Sensor_s structure contains as5600 info
 */
typedef struct
{
	// minimum time between updates to velocity
	float min_elapsed_time; // default is 100 microseconds, or 10kHz

	// velocity calculation variables
	float velocity;
	float angle_prev; // result of last call to getSensorAngle(), used for full rotations and velocity
	long angle_prev_ts; // timestamp of last call to getAngle, used for velocity
	float vel_angle_prev; // angle at last call to getVelocity, used for velocity
	long vel_angle_prev_ts; // last velocity calculation timestamp
	int32_t full_rotations; // full rotation tracking
	int32_t vel_full_rotations; // previous full rotation value for velocity calculation
	float sensor_offset;
	int sensor_direction;
} Sensor_s;


/*
 * @brief : Sensor Initialization
 * @param :
 *          sens --> pointer to Sensor_s structure
*/
void sensor_Init(Sensor_s *sens);

/*
 * @brief  : Sensor Read Mechanical Shaft Angle
 * @retval : read angle from as5600
*/
float sensor_GetSensorAngle();

/*
 * @brief : Sensor Get Mechanical Shaft Angle
 * @param :
 *          sens --> pointer to Sensor_s structure
*/
float sensor_GetMechAngle(Sensor_s *sens);

/*
 * @brief : Sensor Get Electrical Angle
 * @param :
 *          sens            --> pointer to Sensor_s structure
 *          zero_elec_angle --> zero electrical angle
 *          pole_pairs      --> BLDC motor pole-pairs
*/
float sensor_GetElecAngle(Sensor_s *sens, float zero_elec_angle, int pole_pairs);

/*
 * @brief : Sensor Get Angle including full rotations
 * @param :
 *          sens --> pointer to Sensor_s structure
*/
float sensor_GetAngle(Sensor_s *sens);

/*
 * @brief : Sensor Update
 * @param :
 *          sens --> pointer to Sensor_s structure
*/
void sensor_Update(Sensor_s *sens);

/*
 * @brief  : Sensor filtered angle
 * @param  :
 *           LPF_angle --> pointer to LowPassFilter_s contains low-pass filter operator
 *           sens      --> pointer to Sensor_s structure
 * @retval : filtered angle data
*/
float sensor_ShaftAngle(LowPassFilter_s *LPF_angle, Sensor_s *sens);

/*
 * @brief  : Sensor normalizing radian angle to [0,2PI]
 * @param  :
 *           angle --> angle in radians (maybe out of [0,2PI])
 * @retval : normalized angle
*/
__attribute__((weak)) float sensor_NormalizeAngle(float angle);

/*
 * @brief  : Sensor filtered velocity
 * @param  :
 *           LPF_velocity --> pointer to LowPassFilter_s contains low-pass filter operator
 *           sens     --> pointer to Sensor_s structure
 * @retval : filtered velocity data
*/
float sensor_ShaftVelocity(LowPassFilter_s *LPF_velocity, Sensor_s *sens);

/*
 * @brief  : Sensor Get Velocity including full rotations velocity
 * @param  :
 *           sens --> pointer to Sensor_s structure
 * @retval : full rotation velocity
*/
float sensor_GetVelocity(Sensor_s *sens);

#endif /* SENSOR_INC_SENSOR_H_ */

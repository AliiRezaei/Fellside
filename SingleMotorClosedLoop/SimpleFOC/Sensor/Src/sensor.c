/*
 * sensor.c
 *
 *  Created on: Apr 24, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief : Sensor Initialization
 * @param :
 *          sens --> pointer to Sensor_s structure
*/
void sensor_Init(Sensor_s *sens)
 {
	// minimum time between updates to velocity
	sens->min_elapsed_time = 0.000100f; // default is 100 microseconds, or 10kHz

	// velocity calculation variables
	sens->velocity = 0.0f;
	sens->angle_prev = 0.0f; // result of last call to getSensorAngle(), used for full rotations and velocity
	sens->angle_prev_ts = 0; // timestamp of last call to getAngle, used for velocity
	sens->vel_angle_prev = 0.0f; // angle at last call to getVelocity, used for velocity
	sens->vel_angle_prev_ts = 0; // last velocity calculation timestamp
	sens->full_rotations = 0; // full rotation tracking
	sens->vel_full_rotations = 0; // previous full rotation value for velocity calculation

	// initialize all the internal variables of Sensor to ensure a "smooth" startup (without a 'jump' from zero)
	sensor_GetSensorAngle(); // call once
	tim_TIM2_Delay_us(1);   // 1 us delay
	sens->vel_angle_prev = sensor_GetSensorAngle(); // call again
	sens->vel_angle_prev_ts = _tim_usec_val_();
	tim_TIM2_Delay_ms(1);  // 1 ms delay
	sensor_GetSensorAngle(); // call once
	tim_TIM2_Delay_us(1);  // 1 us delay
	sens->angle_prev = sensor_GetSensorAngle(); // call again
	sens->angle_prev_ts = _tim_usec_val_();

	sens->sensor_offset = 0.0f;

	// look at here
	sens->sensor_direction = 1;
}

/*
 * @brief  : Sensor Read Mechanical Shaft Angle
 * @retval : mech angle
*/
float sensor_GetSensorAngle()
{
	// look at here
	uint16_t raw_angle = as5600_ReadRawAngle();
	return (float) (raw_angle * _2PI / _powtwo(12));
//	return (float) (raw_angle * AS5600_PRECISE * _PI / 180.0);
}

/*
 * @brief : Sensor Get Mechanical Shaft Angle
 * @param :
 *          sens --> pointer to Sensor_s structure
*/
float sensor_GetMechAngle(Sensor_s *sens)
{
	return sens->angle_prev;
}

/*
 * @brief : Sensor Get Electrical Angle
 * @param :
 *          sens            --> pointer to Sensor_s structure
 *          zero_elec_angle --> zero electrical angle
 *          pole_pairs      --> BLDC motor pole-pairs
*/
float sensor_GetElecAngle(Sensor_s *sens, float zero_elec_angle, int pole_pairs)
{
	return sensor_NormalizeAngle((float)(sens->sensor_direction * pole_pairs) * sensor_GetMechAngle(sens) - zero_elec_angle);
}

/*
 * @brief : Sensor Get Angle including full rotations
 * @param :
 *          sens --> pointer to Sensor_s structure
*/
float sensor_GetAngle(Sensor_s *sens)
{
    return (float)sens->full_rotations * _2PI + sens->angle_prev;
}

/*
 * @brief : Sensor Update
 * @param :
 *          sens --> pointer to Sensor_s structure
*/
void sensor_Update(Sensor_s *sens)
{
    float val = sensor_GetSensorAngle();
    if (val<0) // sensor angles are strictly non-negative. Negative values are used to signal errors.
        return; // TODO signal error, e.g. via a flag and counter
    sens->angle_prev_ts = _tim_usec_val_();
    float d_angle = val - sens->angle_prev;
    // if overflow happened track it as full rotation
    if(fabs(d_angle) > (0.8f*_2PI) ) sens->full_rotations += ( d_angle > 0 ) ? -1 : 1;
    sens->angle_prev = val;
}

/*
 * @brief  : Sensor filtered angle
 * @param  :
 *           FOCMotor --> pointer to FOCMotor_s contains low-pass filter operator
 *           sens     --> pointer to Sensor_s structure
 * @retval : filtered angle data
*/
float sensor_ShaftAngle(BLDCMotor_s *BLDCMotor, Sensor_s *sens)
{
	return sens->sensor_direction
			* lpf_Operator(&(BLDCMotor->LPF_angle), sensor_GetAngle(sens))
			- sens->sensor_offset;
}

/*
 * @brief  : Sensor normalizing radian angle to [0,2PI]
 * @param  :
 *           angle --> angle in radians (maybe out of [0,2PI])
 * @retval : normalized angle
*/
__attribute__((weak)) float sensor_NormalizeAngle(float angle)
{
  float a = fmod(angle, _2PI);
  return a >= 0 ? a : (a + _2PI);
}

/*
 * @brief  : Sensor filtered velocity
 * @param  :
 *           FOCMotor --> pointer to FOCMotor_s contains low-pass filter operator
 *           sens     --> pointer to Sensor_s structure
 * @retval : filtered velocity data
*/
float sensor_ShaftVelocity(BLDCMotor_s *BLDCMotor, Sensor_s *sens)
{
  return sens->sensor_direction*lpf_Operator(&(BLDCMotor->LPF_velocity), sensor_GetVelocity(sens));
}

/*
 * @brief  : Sensor Get Velocity including full rotations velocity
 * @param  :
 *           sens --> pointer to Sensor_s structure
 * @retval : full rotation velocity
*/
float sensor_GetVelocity(Sensor_s *sens)
{
    // calculate sample time
    float Ts = (sens->angle_prev_ts - sens->vel_angle_prev_ts)*1e-6f;
    if (Ts < 0.0f) {    // handle micros() overflow - we need to reset vel_angle_prev_ts
    	sens->vel_angle_prev = sens->angle_prev;
    	sens->vel_full_rotations = sens->full_rotations;
    	sens->vel_angle_prev_ts = sens->angle_prev_ts;
        return sens->velocity;
    }
    if (Ts < sens->min_elapsed_time) return sens->velocity; // don't update velocity if deltaT is too small

    sens->velocity = ( (float)(sens->full_rotations - sens->vel_full_rotations)*_2PI + (sens->angle_prev - sens->vel_angle_prev) ) / Ts;
    sens->vel_angle_prev = sens->angle_prev;
    sens->vel_full_rotations = sens->full_rotations;
    sens->vel_angle_prev_ts = sens->angle_prev_ts;

    return sens->velocity;
}

/*
 * BLDCDriver.h
 *
 *  Created on: May 18, 2025
 *      Author: Ali Rezaei
 */

#ifndef DRIVER_INC_BLDCDRIVER_H_
#define DRIVER_INC_BLDCDRIVER_H_

typedef struct
 {
	float voltage_power_supply; // power supply voltage
	float voltage_limit;        // limiting voltage set to the motor
	float dc_a;                 // currently set duty cycle on phaseA
	float dc_b;                 // currently set duty cycle on phaseB
	float dc_c;                 // currently set duty cycle on phaseC
} BLDCDriver_s;

/*
 * @brief : BLDC Driver Initialization
 * @param :
 *         BLDCDriver --> pointer to BLDCDriver_s structure
 */
void BLDCDriver_Init(BLDCDriver_s *BLDCDriver);

/*
 * @brief : BLDC Driver Set PWM
 * @param :
 *         BLDCDriver --> pointer to BLDCDriver_s structure
 *         Ua         --> phase A voltage
 *         Ub         --> phase B voltage
 *         Uc         --> phase C voltage
 */
void BLDCDriver_SetPWM(BLDCDriver_s *BLDCDriver, float Ua, float Ub, float Uc);

/*
 * @brief : BLDC Driver Write Duty Cycle in proper Timer
 * @param :
 *         BLDCDriver --> pointer to BLDCDriver_s structure
 */
void BLDCDriver_WriteDutyCycle(BLDCDriver_s *BLDCDriver);

#endif /* DRIVER_INC_BLDCDRIVER_H_ */

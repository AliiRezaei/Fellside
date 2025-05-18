/*
 * BLDCDriver.c
 *
 *  Created on: May 18, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief : BLDC Driver Initialization
 * @param :
 *         BLDCDriver --> pointer to BLDCDriver_s structure
 */
void BLDCDriver_Init(BLDCDriver_s *BLDCDriver)
{
	// default power-supply value
	BLDCDriver->voltage_power_supply = DEF_POWER_SUPPLY;
	BLDCDriver->voltage_limit        = DEF_DRV_VOLT_LIM;
}

/*
 * @brief : BLDC Driver Set PWM
 * @param :
 *         BLDCDriver --> pointer to BLDCDriver_s structure
 *         Ua         --> phase A voltage
 *         Ub         --> phase B voltage
 *         Uc         --> phase C voltage
 */
void BLDCDriver_SetPWM(BLDCDriver_s *BLDCDriver, float Ua, float Ub, float Uc)
{
	// limit the voltage in driver
	Ua = _constrain(Ua, 0.0f, BLDCDriver->voltage_limit);
	Ub = _constrain(Ub, 0.0f, BLDCDriver->voltage_limit);
	Uc = _constrain(Uc, 0.0f, BLDCDriver->voltage_limit);

	// calculate duty cycle limited in [0,1]
	BLDCDriver->dc_a = _constrain(
			Ua / BLDCDriver->voltage_power_supply, 0.0f, 1.0f);
	BLDCDriver->dc_b = _constrain(
			Ub / BLDCDriver->voltage_power_supply, 0.0f, 1.0f);
	BLDCDriver->dc_c = _constrain(
			Uc / BLDCDriver->voltage_power_supply, 0.0f, 1.0f);

	// write in proper timer
	BLDCDriver_WriteDutyCycle(BLDCDriver);
}

/*
 * @brief : BLDC Driver Write Duty Cycle in proper Timer
 * @param :
 *         BLDCDriver --> pointer to BLDCDriver_s structure
 */
void BLDCDriver_WriteDutyCycle(BLDCDriver_s *BLDCDriver)
{
	// PWM counts
	uint32_t ticks = TIM3->ARR + 1;

	// write in timer 3
	TIM3->CCR1 = (ticks * BLDCDriver->dc_a);
	TIM3->CCR2 = (ticks * BLDCDriver->dc_b);
	TIM3->CCR3 = (ticks * BLDCDriver->dc_c);
}

/*
 * as5600.c
 *
 *  Created on: Mar 9, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief : as5600 status check
 */
void as5600_Status(void)
{
	// read status register
	uint8_t status = i2c_I2C1_Read(AS5600_ADDR, AS5600_STATUS_REG);

	// MH bit
	bool MH = status & AS5600_STATUS_MH;
	if(MH) {printf("AGC minimum gain overflow, magnet too strong.\n");}

	// ML bit
	bool ML = status & AS5600_STATUS_ML;
	if(ML) {printf("AGC maximum gain overflow, magnet too weak.\n");}

	// MD bit
	bool MD = status & AS5600_STATUS_MD;
	if(MD) {printf("Magnet was detected.\n");}
}

/*
 * @brief  : as5600 read raw angle resister
 * @retval : raw angle register data
 */
uint16_t as5600_ReadRawAngle(void)
{
	// read MSB
	uint8_t MSB   = i2c_I2C1_Read(AS5600_ADDR, AS5600_RAWANGLE_REG_MSB);

	// read LSB
	uint8_t LSB   = i2c_I2C1_Read(AS5600_ADDR, AS5600_RAWANGLE_REG_LSB);

	// construct data and mask it in 12 bit
	uint16_t data = ((MSB << 8) | LSB) & 0xFFF;
	return data;
}

/*
 * @brief  : as5600 read angle resister
 * @retval : angle register data
 */
uint16_t as5600_ReadAngle(void)
{
	// read MSB
	uint8_t MSB = i2c_I2C1_Read(AS5600_ADDR, AS5600_ANGLE_REG_MSB);

	// read LSB
	uint8_t LSB = i2c_I2C1_Read(AS5600_ADDR, AS5600_ANGLE_REG_LSB);

	// construct data and mask it in 12 bit
	uint16_t data = ((MSB << 8) | LSB) & 0xFFF;
	return data;
}

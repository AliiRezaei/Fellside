/*
 * as5600.h
 *
 *  Created on: Mar 9, 2025
 *      Author: Ali Rezaei
 */

#ifndef INC_AS5600_H_
#define INC_AS5600_H_

/* AS5600 Register Addresses */
#define AS5600_ADDR             (uint8_t)(0x36)      // AS5600 I2C address (7-bit)
#define AS5600_ANGLE_REG_MSB    (uint8_t)(0x0E)      // register for angle (MSB)
#define AS5600_ANGLE_REG_LSB    (uint8_t)(0x0F)      // register for angle (LSB)
#define AS5600_RAWANGLE_REG_MSB (uint8_t)(0x0C)      // register for raw angle (MSB)
#define AS5600_RAWANGLE_REG_LSB (uint8_t)(0x0D)      // register for raw angle (LSB)
#define AS5600_STATUS_REG       (uint8_t)(0x0B)      // status register

/* Bit definition for AS5600_STATUS_REG register */
#define AS5600_STATUS_MH_Pos     (3U)
#define AS5600_STATUS_MH         (1U << AS5600_STATUS_MH_Pos)
#define AS5600_STATUS_ML_Pos     (4U)
#define AS5600_STATUS_ML         (1U << AS5600_STATUS_ML_Pos)
#define AS5600_STATUS_MD_Pos     (5U)
#define AS5600_STATUS_MD         (1U << AS5600_STATUS_MD_Pos)


/*
 * @brief : as5600 status check
 */
void as5600_Status(void);

/*
 * @brief  : as5600 read raw angle resister
 * @retval : raw angle register data
 */
uint16_t as5600_ReadRawAngle(void);

/*
 * @brief  : as5600 read angle resister
 * @retval : angle register data
 */
uint16_t as5600_ReadAngle(void);

#endif /* INC_AS5600_H_ */

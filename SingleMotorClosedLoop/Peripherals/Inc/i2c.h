/*
 * i2c.h
 *
 *  Created on: May 19, 2025
 *      Author: Ali Rezaei
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

/*
 * @brief : I2C1 configuration for communicating with as5600
 * @note  : PB6 --> SCL, PB7 --> SDA
 */
void i2c_I2C1_Config(void);

/*
 * @brief : I2C1 write in proper register
 * @param :
 *          addr --> device i2c address
 *          reg  --> device register for writing
 */
void i2c_I2C1_Write(uint8_t addr, uint8_t reg);

/*
 * @brief : I2C1 read from proper register
 * @param :
 *          addr --> device i2c address
 *          reg  --> device register for reading
 */
uint8_t i2c_I2C1_Read(uint8_t addr, uint8_t reg);

#endif /* INC_I2C_H_ */

/*
 * i2c.c
 *
 *  Created on: May 19, 2025
 *      Author: Ali Rezaei
 */

#include "main.h"

/*
 * @brief : I2C1 configuration for communicating with as5600
 * @note  : PB6 --> SCL, PB7 --> SDA
 */
void i2c_I2C1_Config(void)
{
	// enable GPIOB and I2C1 clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // enable GPIOB clock
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;  // enable I2C1 clock

	// configure PB6 and PB7 as Alternate Function (AF4 for I2C1)
	GPIOB->MODER  &=~ (GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
	GPIOB->MODER  |=  (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);
	GPIOB->OTYPER |=  (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);
	GPIOB->AFR[0] |=  (4 << GPIO_AFRL_AFSEL6_Pos) | (4 << GPIO_AFRL_AFSEL7_Pos);

	// reset I2C1
	I2C1->CR1 |=  I2C_CR1_SWRST;
	I2C1->CR1 &=~ I2C_CR1_SWRST;

	// set I2C1 frequency (APB1 clock = 42 MHz)
	I2C1->CR2 = 42;

	// configure I2C clock control for 400 kHz (Fast Mode)
	I2C1->CCR   = 210;
	I2C1->TRISE = 43;

	// enable I2C1 peripheral
	I2C1->CR1 |= I2C_CR1_PE;
}

/*
 * @brief : I2C1 write in proper register
 * @param :
 *          addr --> device i2c address
 *          reg  --> device register for writing
 */
void i2c_I2C1_Write(uint8_t addr, uint8_t reg)
{
    // start condition
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));

    // send device address with write (0)
    I2C1->DR = (addr << 1);              // shift slave address
    while(!(I2C1->SR1 & I2C_SR1_ADDR));  // wait for send slave address
    (void)I2C1->SR2;                     // clear ADDR flag

    // send register address
    I2C1->DR = reg;                      // slave register address
    while(!(I2C1->SR1 & I2C_SR1_TXE));   // wait for complete transmission

    // stop condition
    I2C1->CR1 |= I2C_CR1_STOP;
}

/*
 * @brief : I2C1 read from proper register
 * @param :
 *          addr --> device i2c address
 *          reg  --> device register for reading
 */
uint8_t i2c_I2C1_Read(uint8_t addr, uint8_t reg)
{
	// init data
    uint8_t data = 0;

    // write register address
    i2c_I2C1_Write(addr, reg);

    // restart condition for reading
    I2C1->CR1 |= I2C_CR1_START; 	     // start condition
    while(!(I2C1->SR1 & I2C_SR1_SB));    // wait for it

    // send device address with read (1)
    I2C1->DR = (addr << 1) | 1;          // shift address with read bit
    while(!(I2C1->SR1 & I2C_SR1_ADDR));  // wait for complete transmission
    (void)I2C1->SR2;                     // clear ADDR flag

    // read MSB
    while(!(I2C1->SR1 & I2C_SR1_RXNE));
    data = (uint8_t)(I2C1->DR & 0xff);

    // stop condition
    I2C1->CR1 |= I2C_CR1_STOP;

    return data;
}

/**
 * @file    i2c.h
 * @brief   This header file consists of functions of I2C protocol
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 *1) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8
 */


#ifndef I2C_H_
#define I2C_H_


#include <stdint.h>

#define I2C_M_START 	I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  	I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 	I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN			I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC				I2C0->C1 &= ~I2C_C1_TX_MASK

#define BUSY_ACK 	    while(I2C0->S & 0x01)
#define TRANS_COMP		while(!(I2C0->S & 0x80))
#define I2C_WAIT 			i2c_wait();

#define NACK 	        I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C0->C1 &= ~I2C_C1_TXAK_MASK


/*
 * @brief Initialize I2C protocol
 *
 * @return void
 */
void i2c_init(void);

/*
 * @brief Defining the I2C protocol
 *
 * @return void
 */
void i2c_busy(void);

/*
 * @brief send i2c start sequence
 *
 * @return void
 */

void i2c_start(void);

/*
 * @brief Initializing the I2C read function
 * @param1 dev Device Address
 * @param2 address Read Address
 * @return void
 */


void i2c_read_setup(uint8_t dev, uint8_t address);

/*
 * @brief Do I2C repeated read
 * @param1 The last read data
 * @return void
 */

uint8_t i2c_repeated_read(uint8_t);

/*
 * @brief Read the I2C read byte
 * @param1 device address
 * @param2 read address
 * @return void
 */

uint8_t i2c_read_byte(uint8_t dev, uint8_t address);


/*
 * @brief Wrtie the I2C byte
 * @param1 device address
 * @param2 read address
 * @param3 data
 * @return void
 */

void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);


#endif /* I2C_H_ */

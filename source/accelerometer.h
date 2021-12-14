/**
 * @file    acclerometer.h
 * @brief   This header file consists of functions prototypes to get accelerometer values through I2C protocol
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 *1) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8
 */



#ifndef MMA8451_H_
#define MMA8451_H_

#include <stdint.h>

#define MMA_ADDR 0x3A
#define REG_XHI 0x01
#define REG_CTRL1  0x2A
#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)


/*
 * @brief Initializes the acclerometer
 *
 * @return TRUE if accelerometer works through I2C, FALSE if not works
 */

int init_mma(void);

/*
 * @brief Funciton to get the roll
 *
 * @return roll value
 */

int get_roll();


#endif /* MMA8451_H_ */

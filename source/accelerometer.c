/**
 * @file    acclerometer.c
 * @brief   This source file consists of functions definitions to get accelerometer values through  I2C protocol
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 *1) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8
 */

#include <MKL25Z4.H>
#include "accelerometer.h"
#include "i2c.h"
#include <math.h>
#include "fsl_debug_console.h"
#include "stdio.h"

int16_t acc_X=0, acc_Y=0, acc_Z=0;
float roll=0.0, pitch=0.0;

/*
 * @brief Initializes the acclerometer
 *
 * @return TRUE if accelerometer works through I2C, FALSE if not works
 */

int init_mma()
{
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);  /*set active mode, 14 bit samples and 800 Hz ODR*/
	return 1;
}


/*
 * @brief Funciton to get the roll
 *
 * @return roll value
 */

int get_roll()
{
	int i;
	uint8_t data[6];
	int16_t temp[3];
	int roll=0;
	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	for( i=0; i<5; i++)
	{
		data[i] = i2c_repeated_read(0);					/* Read five bytes in repeated mode */
	}

	data[i] = i2c_repeated_read(1);						/*Read last byte */

	for ( i=0; i<3; i++ )
	{
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;								/*14 bits alignment*/
	acc_Z = temp[2]/4;

	 float ay = acc_Y/COUNTS_PER_G,
		   az = acc_Z/COUNTS_PER_G;

	roll = atan2(ay, az)*180/M_PI;				/*Formula to calculte roll*/
	return roll;


}


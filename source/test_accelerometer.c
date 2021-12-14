/**
 * @file    test_acclerometer.c
 * @brief   This source file consists of function definition to test the accelerometer
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 */

#include "test_accelerometer.h"
#include "accelerometer.h"
#include "timer.h"
#include "stdio.h"
#include <stdlib.h>

#define FIVE_SECONDS 5000
#define DEGREE_45    45
#define DEGREE_90    90
#define DEGREE_135	135

#define PASS 1
#define FAIL 0


/*
 * @brief Function to check whether the accelrometer is working or not
 *
 * @return TRUE if accelerometer work, FALSE if not works
 */

int test_accelerometer()
{
	int result = FAIL;
	int count =0;
	printf("-------------Testing I2C and accelerometer ---------------\n\r");
	printf("1. Tilt the board to 45 degrees within 5 seconds\n\r");
	reset_timer();
	while( get_timer() < FIVE_SECONDS)							/*Waiting for 5 seconds*/
	{
		if (abs(get_roll()) == DEGREE_45)
		{
			count = count + 1;
			printf("45 degree is reached\n\r");
			break;
		}

	}

	printf("2. Tilt the board to 90 degrees within 5 seconds\n\r");
	reset_timer();
	while( get_timer() < FIVE_SECONDS)
	{
		if (abs(get_roll()) == DEGREE_90)
		{
			count = count + 1;
			printf("90 degree is reached\n\r");
			break;
		}

	}

	printf("3. Tilt the board to 135 degrees within 5 seconds\n\r");
	reset_timer();
	while( get_timer() < FIVE_SECONDS)
	{
		if (abs(get_roll()) == DEGREE_135)
		{
			count = count + 1;
			printf("135 degree is reached\n\r");
			break;
		}
	}

	if(count != 3)
	{
		printf("Timeout error while measuring 45/90/135 degree  / Error in fetching accelerometer through I2C bus \n\r");
		printf(" --------------------------------------------------------------------------------------------------\n\r\n\r");
	}
	else if (count == 3)
	{
		printf("Accelerometer data is fetched successfully through I2C bus\n\r");
		printf("----------------------------------------------------------\n\r\n\r");			/*Checking whether all the angle are measured*/
		result = PASS;
	}

	return result;
}

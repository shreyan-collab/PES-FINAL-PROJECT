/**
 * @file    statemachine.c
 * @brief   This source file consists of function definition of state machine which executed the digital gauge meter
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 */
#include <accelerometer.h>
#include <LEDs.h>
#include <stdio.h>
#include "fsl_debug_console.h"
#include "commandprocessor.h"
#include "statemachine.h"
#include "timer.h"
#include "switch.h"
#include "i2c.h"
#include "LEDs.h"
#include "accelerometer.h"
#include "sysclock.h"
#include "uart.h"
#include "test_cbfifo.h"
#include "test_switch.h"
#include "test_cbfifo.h"
#include "test_leds.h"
#include "test_accelerometer.h"

#define PASS 1						/*To store the result of test function*/
#define FAIL 0

typedef enum
{
	INITIALIZE=0,
	TEST,
	GET_INPUT

}state_t;


/*
 * @brief Function to initialize peripherals used to in this project
 *
 * @return void
 */
static void initialize_peripherals()
{
   	sysclock_init();								/*Initializing the system clock as per UART requirements*/
	init_uart0();								    /*Initializes the UART 0 peripheral of KL25Z board*/
	Init_Red_LED_PWM(PWM_PERIOD);					/*Initializes the Timer PWM module 2 channel 0 connected to red led (Port B 18)*/
	Init_Green_LED_PWM(PWM_PERIOD);					/* Initializes the Timer PWM module 2 channel 1 connected to green led (Port B 19)*/
	Init_Blue_LED_PWM(PWM_PERIOD);					/* Initializes the Timer PWM module 0 channel 1 connected to blue led (Port D 1)*/
	i2c_init();										/* Initialize i2c*/
	init_mma();										/* Initialize the accelerometer*/
	init_switch();									/* Initialize the GPIO switch*/
	Init_SysTick();									/* Initialize the systick timer used to calculate delay*/

}

/*
 * @brief Test function to test peripherals
 *
 * @return void
 */
static void test()
{
	int count = 0;
	printf("-----------Testing the Digital Gauge peripherals----------\n\r");
	printf("1. Testing Switch\n\r");
	printf("2. Testing CBFIFO\n\r");
	printf("3. Testing LED's\n\r");
	printf("4. Testing Accelerometer\n\r");
	printf("----------------------------------------------------------\n\r\n\r");
	if(test_switch())
	{
		count = count + 1;
	}
	if(test_cbfifo())
	{
		count = count + 1;
	}
	if(test_leds())
	{
		count = count + 1;
	}
	if(test_accelerometer())
	{
		count = count + 1;

	}
	if (count == 4)
	{
		printf("All peripheral test cases are passed successfully \n\r");		/*If all 4 are true, all peripherals are working properly*/
	}
	else
	{
		printf("All peripheral test cases are not passed successfully\n\r");
	}

}




/*
 * @brief The state machine for digial guage meter
 *
 * @return void
 */
void statemachine()
{
	state_t currentState = INITIALIZE;
	while(1)
	{
		switch (currentState)
		{
			case INITIALIZE:
				initialize_peripherals();
				currentState = TEST;
			break;

			case TEST:
				test();
				currentState = GET_INPUT;
			break;

			case GET_INPUT:
				printf("? ");
				accumulator();
			break;
		}
	}
}


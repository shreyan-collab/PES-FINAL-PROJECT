/**
 * @file    test_switch.c
 * @brief   This source file consists of function definition to test the switch
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 */
#include "switch.h"
#include "test_switch.h"
#include "stdio.h"
#include "timer.h"
#include "LEDs.h"
#define FIVE_SECONDS 5000

#define PASS 1
#define FAIL 0
#define GREEN 0xFF
#define OFF  0


/*
 * @brief Function to check whether the switch works or not
 *
 * @return TRUE if switch work, FALSE if not works
 */
bool test_switch()
{
	int result = FAIL;
	printf("-----------------Testing the switch-----------------\n\r");
	printf("Press the Switch\n\r");
	reset_timer();
	reset_switch();
	while( get_timer() < FIVE_SECONDS)													/*Wait for 5 seconds*/
	{
		if (check_switch_pressed())
		{
			update_led_colour(OFF, GREEN, OFF);									/*Indicate Green colour for switch pressed*/
			result = PASS;
			printf("Switch is pressed\n\r");
			printf("-----------------------------------------------------\n\r\n\r");
			break;
		}
	}
	if (result == FAIL)
	{
		printf("Switch is not pressed /  check if switch is connected to the board\n\r");
		printf("-------------------------------------------------------------------\n\r\n\r");
	}
	delay(2000);
	update_led_colour(OFF,OFF,OFF);
	return result;
}

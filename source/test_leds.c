/**
 * @file    test_leds.c
 * @brief   This source file consists of function definition to test the leds
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 */

#include "LEDs.h"
#include <stdio.h>
#include "timer.h"

#define RED   0xFF
#define BLUE  0xFF
#define GREEN 0xFF
#define OFF   0x00

#define ONE_SECOND 1000
#define TWO_SECOND 2000
#define PASS 1


/*
 * @brief Function to check whether the led's are working or not
 *
 * @return TRUE if leds work, FALSE if not works
 */

int test_leds()
{
	delay(TWO_SECOND);
	printf("----------------------Testing Leds-----------------------\n\r");
	printf("---Blinking RED, BLUE, GREEN, WHITE LED for one second---\n\r");
	update_led_colour(RED, OFF, OFF);				/*Turning On red, blue, greem for 1 second*/
	delay(ONE_SECOND);
	update_led_colour(OFF, BLUE, OFF);
	delay(ONE_SECOND);
	update_led_colour(OFF, OFF, GREEN);
	delay(ONE_SECOND);
	update_led_colour(RED, BLUE, GREEN);
	delay(ONE_SECOND);
	update_led_colour(OFF, OFF, OFF);				/*Combination of Red, Blue, Green colour at the same time*/
	delay(ONE_SECOND);
	printf("----------All the led colours are blinking properly-------\n\r");
	printf("----------------------------------------------------------\n\r\n\r");
	return PASS;
}

/**
 * @file    pwm.h
 * @brief   This source file consists function prototypes of initializing of TPM modules
 * 			and updating Red, Blue, Green colours through PWM signal
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 * 1) Howdy's Presentation 10,13,14 - To set GPIO port as an input, statemachine, interrupts
 * https://canvas.colorado.edu/courses/75704/files/folder/Lectures
 * 2) Embedded Systems fundamentals with ARM Cortex-M based microcontroller by Alexander Dean
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "MKL25Z4.h"

#define PWM_PERIOD (48000) /*To load MOD value with 48000 for 500 Hz PWM frequency*/

/*
 * @brief: Initializes the Timer PWM module 0 channel 1 connected to blue led (Port D 1)
 * @param: Loading the MOD value with 48000 for 500 Hz PWM frequency
 * @return:void
 */
void Init_Blue_LED_PWM(uint16_t period);

/*
 * @brief: Initializes the Timer PWM module 2 channel 0 connected to red led (Port B 18)
 * @param: Loading the MOD value with 48000 for 500 Hz PWM frequency
 * @return:void
 */
void Init_Red_LED_PWM(uint16_t period);

/*
 * @brief: Initializes the Timer PWM module 2 channel 1 connected to green led (Port B 19)
 * @param: Loading the MOD value with 48000 for 500 Hz PWM frequency
 * @return:void
 */
void Init_Green_LED_PWM(uint16_t period);


/*
 * @brief: Updating the on-board Red, Blue, Green colors through PWM signal
 *
 * According to the state machine, the red, blue, green values are updated according to
 * the PWM frequency and multiplied with 0xFF for more led brightness
 *
 * @param1: Red value to be loaded with TPM 2 Channel 0 ranging from 0-255
 * @param2: Green value to be loaded with TPM 2 Channel 1 ranging from 0-255
 * @param3: Blue value to be loaded with TPM 0 Channel 1 ranging from 0-255
 * @return:void
 */
void update_led_colour(uint16_t redValue,uint16_t greenValue,uint16_t blueValue);


#endif /* TIMERS_H_ */

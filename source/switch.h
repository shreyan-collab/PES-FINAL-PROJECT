
/**
 * @file    switch.h
 * @brief   This header file consists of function prototype to configure the switch as
 * 			input, to check whether button is pressed, and the IRQ handler for switch when pressed
 * @date 	10th October, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 * 1) Howdy's Presentation 10,13,14 - To set GPIO port as an input, statemachine, interrupts
 * https://canvas.colorado.edu/courses/75704/files/folder/Lectures
 * 2) Embedded Systems fundamentals with ARM Cortex-M based microcontroller by Alexander Dean
 */
#ifndef SWITCH_H_
#define SWITCH_H_


/*
 * @brief Initialize the on-board switch to trigger cross-walk state of KL25Z freedom development board
 *
 * The GPIO Port D 3rd pin is configured as input with pull-up functionality to trigger
 * cross-walk when button is pressed
 *
 * @return void
 */
void init_switch();

/*
 * @brief To check whether the button is pressed through interrupt
 *
 * When the interrupt is triggered, a flag is set which is stored in a local variable
 * before resetting the flag
 *
 * @return button_pressed - Sends 0 if button not pressed and 1 if button is pressed
 */
int check_switch_pressed();

/*@brief reset unintentional swtich press
 *
 * @return none
 */
void reset_switch();


#endif /* SWITCH_H_ */

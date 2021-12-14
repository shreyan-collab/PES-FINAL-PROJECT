
/**
 * @file    switch.c
 * @brief   This source file consists of function definition to configure the switch as
 * 			input, to check whether button is pressed, and the IRQ handler for switch when pressed
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 * 1) Howdy's Presentation 10,13,14 - To set GPIO port as an input, statemachine, interrupts
 * https://canvas.colorado.edu/courses/75704/files/folder/Lectures
 * 2) Embedded Systems fundamentals with ARM Cortex-M based microcontroller by Alexander Dean
 */

#include <stdbool.h>
#include "MKL25Z4.h"
#include "switch.h"


#define SWITCH_GPIO_PORT GPIOD
#define SWITCH_PIN 3
#define SWITCH_PIN_CTRL_REG PORTD->PCR[SWITCH_PIN]
#define SWITCH_SCGC5_MASK SIM_SCGC5_PORTD_MASK
#define SWITCH_ISFR PORTD->ISFR
#define INTERRUPT_WHEN_LOGIC_ZERO 8

static int interrupt_triggered = 0; /*Flag set when switch interrupt is triggered*/

/*
 * @brief Initialize the on-board switch to trigger cross-walk state of KL25Z freedom development board
 *
 * The GPIO Port D 3rd pin is configured as input with pull-up functionality to trigger
 * cross-walk when button is pressed
 *
 * @return void
 */

void init_switch()
{
  /*Initializing the switch as input*/
  SIM->SCGC5 |= SWITCH_SCGC5_MASK; /*Setting the clock controlled by the System Integration Module for port D*/
  SWITCH_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;/*Masking PortD 1st Pin Control Register with 0x700 to mask 8,9,10 pins*/
  SWITCH_PIN_CTRL_REG |= PORT_PCR_MUX(1);/*Selecting 1 functionality for Port D 1st pin in
  	  	  	  	  	  	  	  	  	  	  PCR register to act as GPIO*/
  SWITCH_PIN_CTRL_REG |= PORT_PCR_PE(1) | PORT_PCR_PS(1);/*Enabling the pull-up configuration for the pin*/
  SWITCH_GPIO_PORT->PDDR &= ~(1 << SWITCH_PIN);/*Setting the data direction to input*/


  SWITCH_PIN_CTRL_REG |=PORT_PCR_IRQC(INTERRUPT_WHEN_LOGIC_ZERO);/*Configuring interrupt for logic zero*/
  NVIC_SetPriority (PORTD_IRQn, 4);/*Setting up NVIC priority greater than 3*/
  NVIC_EnableIRQ(PORTD_IRQn);/*Enabling the interrupt*/
  __enable_irq();/*If the PM bit in PRIMASK register is set,__enable_irq will enable the interrupt*/
}

/*@brief reset unintentional swtich press
 *
 * @return none
 */
void reset_switch()
{
	interrupt_triggered=0;
}

/*
 * @brief To check whether the button is pressed through interrupt
 *
 * When the interrupt is triggered, a flag is set which is stored in a local variable
 * before resetting the flag
 *
 * @return button_pressed - Sends 0 if button not pressed and 1 if button is pressed
 */
int check_switch_pressed(void)
{
		uint32_t masking_state = __get_PRIMASK();
		__disable_irq();						/*Avoiding racing condition by disabling the interrupt*/
		int button_pressed = interrupt_triggered;
		interrupt_triggered = 0;
		__set_PRIMASK(masking_state);			/*Enabling the interrupt*/
		return button_pressed;
}

/*
 * @brief Interrupt routine called when user presses the button connected to PORT D 3rd pin
 *
 * When the interrupt is triggered, a flag is set and the IFSR register is written 1 to
 * clear the interrupt which was set
 *
 * @return void
 */
void PORTD_IRQHandler(void)
{
	if ( ( (SWITCH_ISFR) & (1 << SWITCH_PIN) ) == 0) /*Check if switch is pressed*/
	return;
	interrupt_triggered = 1;
	SWITCH_ISFR &= (1 << SWITCH_PIN); /*Writing 1 will clear the bit 3 PORT D IFSR register*/
}

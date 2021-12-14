/**
 * @file    pwm.c
 * @brief   This source file consists function definitions of initializing of TPM modules
 * 			and updating Red, Blue, Green colours through PWM signal
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 * 1) Howdy's Presentation 10,13,14 - To set GPIO port as an input, statemachine, interrupts
 * https://canvas.colorado.edu/courses/75704/files/folder/Lectures
 * 2) Embedded Systems fundamentals with ARM Cortex-M based microcontroller by Alexander Dean
 */

#include <LEDs.h>
#include <MKL25Z4.h>

#define RED_LED_PIN (18)								/*Macro for port B 18th pin to access it as red led*/
#define RED_LED_PIN_CTRL_REG PORTB->PCR[RED_LED_PIN]/*Program control Register macro for port B 18th pin*/


#define GREEN_LED_PIN (19)							/*Macro for port B 19th pin to access it as green led*/
#define GREEN_LED_PIN_CTRL_REG PORTB->PCR[GREEN_LED_PIN]/*Program control Register macro for port B 19th pin*/

#define BLUE_LED_PIN (1)								/*Macro for port D 1st pin to access it as blue led*/
#define BLUE_LED_PIN_CTRL_REG PORTD->PCR[BLUE_LED_PIN]/*Program control Register macro for port D 1st pin*/

#define CHANNEL_1 	(1)	/*To select the TPM channnel*/
#define CHANNEL_0 	(0)

#define CONTINUE_OPERATION (3)
/*
 * @brief: Initializes the Timer PWM module 0 channel 1 connected to blue led (Port D 1)
 * @param: Loading the MOD value with 48000 for 500 Hz PWM frequency
 * @return:void
 */
void Init_Blue_LED_PWM(uint16_t period)
{
	/*Initialization of clock for port D and configuration of port D 1st pin for TPM*/

	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; /*Setting the clock controlled by the
										System Integration Module for port D*/
	BLUE_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK; /*Masking PortD 1st Pin Control Register with 0x700
	 	 	 	 	 	 	 	 	 	 	 	 to mask 8,9,10 pins*/
	BLUE_LED_PIN_CTRL_REG |= PORT_PCR_MUX(4); /*Selecting 4 functionality for Port D 1st pin
	 	 	 	 	 	 	 	 	 	 	 in PCR register to enable TPM mode*/

	/*Configuring the TPM0 module with channel 1 */
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;/*Setting the clock controlled by the System Integration Module
	 	 	 	 	 	 	 	 	 	 for TPM module*/
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK); /*Setting the clock source for
																	TPM as 48 MHz*/
	TPM0->MOD = period-1; /*Loading the MOD value with 47999 for 500 Hz PWM frequency*/
	TPM0->SC =  TPM_SC_PS(1);/*Configuring TPM as UP counter with a prescaler of 2*/
	TPM0->CONF |= TPM_CONF_DBGMODE(CONTINUE_OPERATION);/*Continuing operation in debug mode*/
	TPM0->CONTROLS[CHANNEL_1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK; /*Setting channel 1 of TPM0
																			to edge-aligned low-true PWM*/
	TPM0->CONTROLS[CHANNEL_1].CnV = 0;/* Initializing the duty cycle with value 0*/
	TPM0->SC |= TPM_SC_CMOD(1); /*Enabling the TPM module*/
}


/*
 * @brief: Initializes the Timer PWM module 2 channel 0 connected to red led (Port B 18)
 * @param: Loading the MOD value with 48000 for 500 Hz PWM frequency
 * @return:void
 */

void Init_Red_LED_PWM(uint16_t period)
{
	/*Initialization of clock for port B and configuration of port B 18th pin for TPM*/

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; /*Setting the clock controlled by the
										System Integration Module for port B*/
	RED_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK; /*Masking PortB 18 Pin Control Register with 0x700
	 	 	 	 	 	 	 	 	 	 	 	 to mask 8,9,10 pins*/
	RED_LED_PIN_CTRL_REG |= PORT_PCR_MUX(3); /*Selecting 3 functionality for Port B 18 pin in
											PCR register to enable TPM mode*/

	/*Configuring the TPM2 module with channel 0 */
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK; /*Setting the clock controlled by the System Integration Module
	 	 	 	 	 	 	 	 	 	 for TPM module*/
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);/*Setting the clock source
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 for TPM as 48 MHz*/
	TPM2->MOD = period-1;/*Loading the MOD value with 47999 for 500 Hz PWM frequency*/
	TPM2->SC =  TPM_SC_PS(1);/*Configuring TPM as UP counter with a prescaler of 2*/
	TPM2->CONF |= TPM_CONF_DBGMODE(CONTINUE_OPERATION);/*Continuing operation in debug mode*/
	TPM2->CONTROLS[CHANNEL_0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK; /*Setting channel 0 of TPM2
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 to edge-aligned low-true PWM*/
	TPM2->CONTROLS[CHANNEL_0].CnV = 0;/* Initializing the duty cycle with value 0*/
	TPM2->SC |= TPM_SC_CMOD(1);/*Enabling the TPM module*/
}

/*
 * @brief: Initializes the Timer PWM module 2 channel 1 connected to green led (Port B 19)
 * @param: Loading the MOD value with 48000 for 500 Hz PWM frequency
 * @return:void
 */
void Init_Green_LED_PWM(uint16_t period)
{
	/*Configuration of port B 19th pin for TPM*/
	GREEN_LED_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK; /*Masking PortB 18 Pin Control Register with 0x700
	 	 	 	 	 	 	 	 	 	 	 	 	 	 to mask 8,9,10 pins*/
	GREEN_LED_PIN_CTRL_REG |= PORT_PCR_MUX(3); /*Selecting 3 functionality for Port B 18 pin in
													PCR register to enable TPM mode*/

	/*Configuring the TPM2 module with channel 1 */
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;/*Setting the clock controlled by the System Integration Module
	 	 	 	 	 	 	 	 	 	 for TPM module*/
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);/*Setting the clock source
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 for TPM as 48 MHz*/
	TPM2->MOD = period-1;/*Loading the MOD value with 47999 for 500 Hz PWM frequency*/
	TPM2->SC =  TPM_SC_PS(1);/*Configuring TPM as UP counter with a prescaler of 2*/
	TPM2->CONF |= TPM_CONF_DBGMODE(CONTINUE_OPERATION);/*Continuing operation in debug mode*/
	TPM2->CONTROLS[CHANNEL_1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;/*Setting channel 0 of TPM2
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 to edge-aligned low-true PWM*/
	TPM2->CONTROLS[CHANNEL_1].CnV = 0; /*Initializing the duty cycle with value 0*/
	TPM2->SC |= TPM_SC_CMOD(1);/*Enabling the TPM module*/
}

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

void update_led_colour(uint16_t redValue1,uint16_t greenValue1,uint16_t blueValue1)
{
	/*Setting the duty cycle for Red, Green, Blue each ranging from 0-255 */

   	TPM2->CONTROLS[0].CnV = redValue1 << 0x08;
   	TPM2->CONTROLS[1].CnV = greenValue1 << 0x08;
   	TPM0->CONTROLS[1].CnV = blueValue1 << 0x08;
}

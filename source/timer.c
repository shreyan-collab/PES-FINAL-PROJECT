/**
 * @file    timer.c
 * @brief   This source file consists of function definition to configure the systick module
 	 	 	systick interrupt and timer functions, used to calculate time in state machine
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 * 1) Howdy's Presentation 10,13,14 - To set GPIO port as an input, statemachine, interrupts
 * https://canvas.colorado.edu/courses/75704/files/folder/Lectures
 * 2) Embedded Systems fundamentals with ARM Cortex-M based microcontroller by Alexander Dean
 */


#include <stdio.h>
#include <stdbool.h>
#include "timer.h"
#include "MKL25Z4.h"


#define SYSTICK_LOAD_VALUE  11000			/*A 24 Mhz clock to tick every 1 msec requires to be loaded with 11000*/
#define SYSTICK_MASK_VALUE  0x7

volatile ticktime ticksCount=0; /*Incremented every 62.5 ms in interrupt handler*/
ticktime reset_time=0; /*Used the get the current time value from a previous Value by subtracting it */


/*
 *@brief Initializes the systick to generate a tick every 62.5 ms
 *
 *The clock is set as external clock and timer is incremented very 62.5ms and the NVIC
 *priority is set as 3
 *
 *@return void
 */
void Init_SysTick(void)
{
  	SysTick->LOAD = SYSTICK_LOAD_VALUE;
  	NVIC_SetPriority(SysTick_IRQn,3);
  	SysTick->VAL=0;
  	SysTick->CTRL= SYSTICK_MASK_VALUE ;
  	//SysTick->CTRL=SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk ;

}

/*
 *@brief The interrupt handler when the interrupt is triggered for 62.5 ms
 *
 *Ticks variable is incremented, a flag is set to 1 when 1 ms is reached
 *
 *@return void
 */
void SysTick_Handler()
{
   ticksCount++;

}

/*
 *@brief Time in msec since startup
 *
 *
 *@return time since program startup in msec to the calling function
 */
static ticktime current_time()
{
	return (ticksCount);
}


/*
 *@brief Calculate the number of ticks since startup, used in functions reset_timer()
 *and get_timer() to calculate number of ticks at various intervals
 *
 *@return ticks since program startup to the calling function where every tick is 62.5 ms
 */
static ticktime now()
{
	return ticksCount;
}

/*
 *@brief This function is used in conjunction with get_timer() function
 *		 to calculate the current time by subtracting the reset_time with now time
 *
 *
 *@return void
 */

void reset_timer()
{
	reset_time=current_time();
}

/*
 *@brief This function is used in conjunction with reset_timer() function
 *		 to calculate the current time by subtracting the reset_time with now time
 *
 *
 *@return the current time in ticks  to the calling function where every tick is 62.5 ms
 */
ticktime get_timer()
{
	return current_time()-reset_time;

}


/*
 *@brief This function is used to calculate a delay of required msec
 *
 *@param the msec delay required
 */
void delay(uint32_t delay_msec)
{

  ticktime current_tick = now();
  while ((now() - current_tick) <= delay_msec) 		/*while loop executed till the time is reached*/
  {
	  __asm volatile("NOP");
  }
 }


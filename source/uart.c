/**
* @file uart.c
* @brief Source file consists of implementation functions of UART 0 peripheral
*
* @author Shreyan Dhananjayan (shdh7090@colorado.edu)
* @date December 10th,2021
* @Tools   MCU Expresso IDE, KL25Z Freedom development board
* @references
* 1) Howdy's Presentation 22
* 2) Embedded Systems fundamentals with ARM Cortex-M based microcontroller by Alexander Dean
*/



#include "UART.h"
#include <stdio.h>
#include "queue.h"

#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 				(24e6)
#define SYS_CLOCK				(24e6)

#define BAUD_RATE    	38400
#define DATA_BIT_MODE		0		/*0 for 8 bit mode and 1 for 9 bit mode*/
#define STOP_BITS		    1		/*0 for 1 stop bit and 1 for 2 stop bit*/
#define CHECK_PARITY        0		/*0 for not to use parity and 1 to use parity check*/

extern Q_T TxQ;
extern Q_T RxQ;



/**
* @brief Initializes the uart0 function
* @param none
* @return none
*/
void init_uart0()
{
	uint16_t sbr;											/*Value to be loaded in baud rate generator*/

	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;						/* Enable clock gating for UART0 and Port A */
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);				  /* Set UART clock to 48 MHz clock */


	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);  /* Set pins to UART0 Rx */
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);  /* Set pins to UART0 Tx */


	sbr = (uint16_t)((SYS_CLOCK)/(BAUD_RATE * UART_OVERSAMPLE_RATE));	/* Set baud rate and oversampling ratio */
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(0); 	/*Disable interrupts for RX */
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_BIT_MODE) | UART0_C1_PE(CHECK_PARITY); /*8 data bit mode, No parity */

	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)  	/*Diable interrupt for errors*/
				| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1); 		/* Clearing error flags */

		UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	NVIC_SetPriority(UART0_IRQn, 2); 		/*Setting interrupt priority to 2*/
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	UART0->C2 |= UART_C2_RIE(1); /*Enable receive interrupts but not transmit data*/

	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);    /* Enable UART receiver and transmitter */

	UART0->S1 &= ~UART0_S1_RDRF_MASK;				/*Clear the UART RDRF flag*/

}


/**
* @brief Initializes the uart0 function
* @param none
* @return none
*/
void UART0_IRQHandler(void)
{
	uint8_t inputCharacter;
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
							UART0_S1_FE_MASK | UART0_S1_PF_MASK;	/* clear the error flags */
		inputCharacter = UART0->D;
	}
	if (UART0->S1 & UART0_S1_RDRF_MASK)
	{
		inputCharacter = UART0->D;									/* receive a character */
	    Q_Enqueue(&RxQ, &inputCharacter, 1);

	}
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && 						/* transmitter interrupt enabled */
			(UART0->S1 & UART0_S1_TDRE_MASK) )
	{
		if (Q_Dequeue(&TxQ, &inputCharacter ,1) == 1)
		{
		 UART0->D = inputCharacter;
		}
		else
		{

			UART0->C2 &= ~UART0_C2_TIE_MASK;						/* Disable transmitter interrupt since queue is empty */
		}
	}
}

/**
* @brief Initializes the uart0 function
* @param1 handle handler to use inbuilt functions like printf
* @param2 The buffer to be printed
* @param3 size of data
* @return 0
*/
int __sys_write(int handle, char *buf, int size)
{
	while(Q_Size(&TxQ)!=0);
	Q_Enqueue(&TxQ,buf,size);
	if(!(UART0->C2 & UART_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}
	return 0;

}

/**
* @brief Initializes the uart0 function
*
* @return the character
*/
int __sys_readc(void)
{
	char a=0;

	while(a==0)
	{
		Q_Dequeue(&RxQ, &a ,1);
	}
	return a;
}


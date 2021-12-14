/**
* @file cbfifo.c
* @brief Source file consists of implementation functions of cbfifo
*
* @author Shreyan Dhananjayan (shdh7090@colorado.edu)
* @date November 11,2021
* @Tools   MCU Expresso IDE, KL25Z Freedom development board
*
* 1) https://www.youtube.com/watch?v=okr-XE8yTO8&t=1s
* To understand the concept circular buffer
* 2) Howdy's Presentation 4 at 2nd September,2021
* To understand a approach of finding length, front, rear pointer for a queue
*/
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include "queue.h"
#define ZERO (0)

#define NOT_SET (0)
#define SET (1)

Q_T TxQ={{0},0,0,0,0,false,true};
Q_T RxQ={{0},0,0,0,0,false,true};


/**
* @brief To check if queue is empty
* @parameter Rx/Tx buffer instance
* @return 1 if queue is empty or 0 if queue is not empty
*/
bool Q_Empty(Q_T *cbfifo)
{
	if((cbfifo->rear==cbfifo->front) && (cbfifo->queueFull == NOT_SET) /*Rear==Front is occuring for both, queue full*/
		&& (cbfifo->queueEmpty == SET)) 		    /*and empty, differentiating them using flags*/

	{
		return true;
	}
	else
	{
		return false;
	}
}


/**
* @brief To check if queue is full
* @parameter Rx/Tx buffer instance
* @return 1 if queue is full or 0 if queue is not full
*/
bool Q_Full(Q_T *cbfifo)
{
	if((cbfifo->queueFull == SET) && (cbfifo->rear == cbfifo->front))/*Rear==Front is occuring for both, queue full*/
	{									/*and empty, differentiating them using flags	*/
		return true;
	}
	else
	{
		return false;
	}
}


/*
 * @brief Returns the number of bytes currently on the FIFO.
 * @parameter Rx/Tx buffer instance
 * @return Number of bytes currently available to be dequeued from the FIFO
 */
int Q_Size(Q_T *cbfifo)
{
	if(Q_Full(cbfifo)) /*Condition to calculate length when queue is empty*/
	{
		return MAX_SIZE;
	}
	return ((cbfifo->rear - cbfifo->front) & (MAX_SIZE - 1));

}

/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 * 	 Q_T*cbfifo		  Rx/Tx buffer instance
 *   buf      		  Pointer to the data
 *   nbyte    		  Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns -1.
 */
int Q_Enqueue(Q_T *cbfifo,void *buf, size_t nbyte)
{
	int i=0; /*local variable to increment till n byte*/
	uint8_t *tempdata= (uint8_t *)buf;
	if(buf == NULL) /*When enqueueing a null value */
	{
		return -1;
	}
	if(nbyte == ZERO) /*when no of bytes to be enqueued is zero*/
	{
		return 0;
	}
	for(;i<nbyte;i++)
	{

		if(Q_Full(cbfifo)==true)
		{
			return 0;				/*Enqueued bytes is zero, as buffer is full*/
		}
		if(Q_Full(cbfifo) != true)
		{
			cbfifo->data_buffer[cbfifo->rear]=*(tempdata+i);
			cbfifo->rear=(cbfifo->rear+1) & (MAX_SIZE-1);
			if(cbfifo->rear == cbfifo->front)		/*When queue is full*/
			{
				cbfifo->queueFull=1;
				return i+1;
			}
		}
	}
	return i;
}


/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   Q_T*cbfifo	Rx/Tx buffer instance
 *   buf     	Destination for the dequeued data
 *   nbyte   	Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
uint8_t Q_Dequeue(Q_T *cbfifo, void *buf, size_t nbyte)
{
	int i=0; /*local variable to increment till n byte */
	int j=0; /*local variable to increment buffer index
				where dequeued data is stored */
	if(nbyte == ZERO) /*when no of bytes to be dequeued is zero */
	{
		return 0;
	}
	for(;i<nbyte;i++,j++)
	{
		if(Q_Empty(cbfifo) == true) /*Dequeueing a empty data
										buffer result in zero*/
		{
			return 0;
		}
		if(Q_Empty(cbfifo) != true)/*Dequeue and copy the data to buffer*/
		{
			*((uint8_t *)buf+j)= cbfifo->data_buffer[cbfifo->front];
			cbfifo->data_buffer[cbfifo->front]='\0';
			cbfifo->front=(cbfifo->front+1) & (MAX_SIZE-1);
			if(cbfifo->rear == cbfifo->front)	/*When Queue is empty*/
			{
				cbfifo->queueEmpty=1;
				cbfifo->queueFull=0;
				return i+1;
			}
		}
	}
	return i;

}

/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
int Q_Capacity()
{
	return MAX_SIZE; /*Returning 128 as it is a statically allocated queue*/
}

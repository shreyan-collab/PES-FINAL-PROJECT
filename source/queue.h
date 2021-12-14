
/**
* @file cbfifo.h
* @brief Header file for cbfifo.c source file (implementation functions)
*
* This source file consists of function prototypes of functions used in cbfifo.c
* @author Shreyan Dhananjayan (shdh7090@colorado.edu)
* @date September 13,2021
* @Tools used Gcc compiler (Use cygwin incase of Windows)
* @References, citations
*
* 1) https://www.geeksforgeeks.org/queue-linked-list-implementation/
* 2) https://www.youtube.com/watch?v=okr-XE8yTO8&t=1s
* 3) Howdy's Presentation 4 at 2nd September,2021
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <MKL25Z4.h>
#include <stdbool.h>

#define MAX_SIZE (256)

typedef struct
{
	uint8_t data_buffer[MAX_SIZE];
	uint8_t front;
	uint8_t rear;
	uint16_t capacity;
	uint16_t length;
	bool queueFull;
	bool queueEmpty;

} volatile Q_T;

/**
* @brief To check if queue is empty
* @parameter Rx/Tx buffer instance
* @return 1 if queue is empty or 0 if queue is not empty
*/
bool Q_Empty(Q_T * cbfifo);

/**
* @brief To check if queue is full
* @parameter Rx/Tx buffer instance
* @return 1 if queue is full or 0 if queue is not full
*/
bool Q_Full(Q_T * cbfifo);

/*
 * @brief Returns the number of bytes currently on the FIFO.
 * @parameter Rx/Tx buffer instance
 * @return Number of bytes currently available to be dequeued from the FIFO
 */
int Q_Size(Q_T * cbfifo);

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
int Q_Enqueue(Q_T * cbfifo, void *buf, size_t nbyte);

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
uint8_t Q_Dequeue(Q_T *cbfifo, void *buf, size_t nbyte);



/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
int Q_Capacity();

#endif // QUEUE_H

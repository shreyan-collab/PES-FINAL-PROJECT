/**
* @file test_cbfifo.c
* @brief Source file consists of test function of cbfifo
* @date 10th November, 2021
* @author Shreyan Dhananjayan (shdh7090@colorado.edu)
* @Tools   MCU Expresso IDE, KL25Z Freedom development board
*/


#include <stdio.h>
#include <assert.h>
#include <string.h>

#include"test_cbfifo.h"
#include "queue.h"


#define ZERO 0
#define ARRAY_SIZE_1 (128)
#define ARRAY_SIZE_2 (1024) //Used for dump function

#define PASS 1
Q_T txbuffer;				/*Instances for tranmit and receive buffer*/
Q_T rxbuffer;

/**
* @brief Function to execute the cbfifo test functions which
* is called from main function
*
* @return 1 if test functions are executed properly
* else zero if there is an error in executing the test functions
*/

bool test_cbfifo()
{
	printf("--Testing CBFIFO for UART functionality command processor--\n\r");
	char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";

  /*Checking cbfifo transmit buffer*/
  char buf[1024];
  const int cap = Q_Capacity();

  // asserts in following 2 lines -- this is not testing the student,
  // it's validating that the test is correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf) > cap);
  assert(cap == 256 || cap == 127);

  assert(Q_Size(&txbuffer) == 0);
  assert(Q_Dequeue(&txbuffer,buf, cap) == 0);
  assert(Q_Dequeue(&txbuffer,buf, 1) == 0);

  // Enqueue 10 bytes, then Dequeue same amt
  assert(Q_Enqueue(&txbuffer,str, 10) == 10);
  assert(Q_Size(&txbuffer) == 10);
  assert(Q_Dequeue(&txbuffer, buf, 10) == 10);
  assert(strncmp(buf, str, 10) == 0);
  assert(Q_Size(&txbuffer) == 0);

  // Enqueue 20 bytes;  Dequeue 5, then another 20
  assert(Q_Enqueue(&txbuffer,str, 20) == 20);
  assert(Q_Size(&txbuffer) == 20);
  assert(Q_Dequeue(&txbuffer,buf, 5) == 5);
  assert(Q_Size(&txbuffer) == 15);
  assert(Q_Dequeue(&txbuffer,buf+5, 20) == 15);
  assert(Q_Size(&txbuffer) == 0);
  assert(strncmp(buf, str, 20) == 0);



  // Add 20 bytes and pull out 18
  assert(Q_Enqueue(&txbuffer,str, 20) == 20);
  assert(Q_Size(&txbuffer) == 20);
  assert(Q_Dequeue(&txbuffer,buf, 18) == 18);
  assert(Q_Size(&txbuffer) == 2);
  assert(strncmp(buf, str, 18) == 0);

  // Now add a bunch of data in 4 chunks
  int chunk_size = (cap-2) / 4;
  for (int i=0; i<4; i++) {
    assert(Q_Enqueue(&txbuffer,str+i*chunk_size, chunk_size) == chunk_size);
    assert(Q_Size(&txbuffer) == (i+1)*chunk_size + 2);
  }
  assert(Q_Size(&txbuffer) == 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  assert(Q_Dequeue(&txbuffer,buf, 2) == 2);
  assert(strncmp(buf, str+18, 2) == 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    assert(Q_Dequeue(&txbuffer,buf+i, 1) == 1);
    assert(Q_Size(&txbuffer) == chunk_size*4 - i - 1);
  }
  assert(strncmp(buf, str, chunk_size*4) == 0);


  // write zero bytes
  assert(Q_Enqueue(&txbuffer,str, 0) == 0);
  assert(Q_Size(&txbuffer) == 0);


  //    Enqueue when read < write:
  //    bytes < CAP-write  (1)
  //    bytes exactly CAP-write  (2)
  //    bytes > CAP-write but < space available (3)
  //    bytes exactly the space available (4)
  //    bytes > space available (5)
  assert(Q_Enqueue(&txbuffer,str, 32) == 32);
  assert(Q_Size(&txbuffer) == 32);
  assert(Q_Dequeue(&txbuffer,buf, 16) == 16);
  assert(Q_Size(&txbuffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(Q_Enqueue(&txbuffer,str+32, 32) == 32);  // (1)
  assert(Q_Size(&txbuffer) == 48);
  assert(Q_Enqueue(&txbuffer,str+64, cap-64) == cap-64);  // (2)
  assert(Q_Size(&txbuffer) == cap-16);
  assert(Q_Dequeue(&txbuffer,buf+16, cap-16) == cap-16);
  assert(strncmp(buf, str, cap) == 0);

  assert(Q_Enqueue(&txbuffer,str, 32) == 32);  // advance so that read < write
  assert(Q_Size(&txbuffer) == 32);
  assert(Q_Dequeue(&txbuffer,buf, 16) == 16);
  assert(Q_Size(&txbuffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(Q_Enqueue(&txbuffer,str+32, cap-20) == cap-20);  // (3)
  assert(Q_Size(&txbuffer) == cap-4);
  assert(Q_Dequeue(&txbuffer,buf, cap-8) == cap-8);
  assert(strncmp(buf, str+16, cap-8) == 0);
  assert(Q_Size(&txbuffer) == 4);
  assert(Q_Dequeue(&txbuffer,buf, 8) == 4);
  assert(strncmp(buf, str+16+cap-8, 4) == 0);
  assert(Q_Size(&txbuffer) == 0);

  assert(Q_Enqueue(&txbuffer,str, 49) == 49);  // advance so that read < write
  assert(Q_Size(&txbuffer) == 49);
  assert(Q_Dequeue(&txbuffer,buf, 16) == 16);
  assert(Q_Size(&txbuffer) == 33);
  assert(strncmp(buf, str, 16) == 0);

   /*Checking Receive Buffer*/

  char buf1[1024];


  // asserts in following 2 lines -- this is not testing the student,
  // it's validating that the test is correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf1) > cap);
 // assert(cap == 128 || cap == 127);

  assert(Q_Size(&rxbuffer) == 0);
  assert(Q_Dequeue(&rxbuffer,buf1, cap) == 0);
  assert(Q_Dequeue(&rxbuffer,buf1, 1) == 0);

  // Enqueue 10 bytes, then Dequeue same amt
  assert(Q_Enqueue(&rxbuffer,str, 10) == 10);
  assert(Q_Size(&rxbuffer) == 10);
  assert(Q_Dequeue(&rxbuffer, buf1, 10) == 10);
  assert(strncmp(buf1, str, 10) == 0);
  assert(Q_Size(&rxbuffer) == 0);

  // Enqueue 20 bytes;  Dequeue 5, then another 20
  assert(Q_Enqueue(&rxbuffer,str, 20) == 20);
  assert(Q_Size(&rxbuffer) == 20);
  assert(Q_Dequeue(&rxbuffer,buf1, 5) == 5);
  assert(Q_Size(&rxbuffer) == 15);
  assert(Q_Dequeue(&rxbuffer,buf1+5, 20) == 15);
  assert(Q_Size(&rxbuffer) == 0);
  assert(strncmp(buf1, str, 20) == 0);



  // Add 20 bytes and pull out 18
  assert(Q_Enqueue(&rxbuffer,str, 20) == 20);
  assert(Q_Size(&rxbuffer) == 20);
  assert(Q_Dequeue(&rxbuffer,buf1, 18) == 18);
  assert(Q_Size(&rxbuffer) == 2);
  assert(strncmp(buf1, str, 18) == 0);

  // Now add a bunch of data in 4 chunks
   chunk_size = (cap-2) / 4;
  for (int i=0; i<4; i++) {
    assert(Q_Enqueue(&rxbuffer,str+i*chunk_size, chunk_size) == chunk_size);
    assert(Q_Size(&rxbuffer) == (i+1)*chunk_size + 2);
  }
  assert(Q_Size(&rxbuffer) == 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  assert(Q_Dequeue(&rxbuffer,buf1, 2) == 2);
  assert(strncmp(buf1, str+18, 2) == 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    assert(Q_Dequeue(&rxbuffer,buf1+i, 1) == 1);
    assert(Q_Size(&rxbuffer) == chunk_size*4 - i - 1);
  }
  assert(strncmp(buf1, str, chunk_size*4) == 0);



  // write zero bytes
  assert(Q_Enqueue(&rxbuffer,str, 0) == 0);
  assert(Q_Size(&rxbuffer) == 0);

  //    Enqueue when read < write:
  //        bytes < CAP-write  (1)
  //        bytes exactly CAP-write  (2)
  //        bytes > CAP-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  assert(Q_Enqueue(&rxbuffer,str, 32) == 32);  // advance so that read < write
  assert(Q_Size(&rxbuffer) == 32);
  assert(Q_Dequeue(&rxbuffer,buf1, 16) == 16);
  assert(Q_Size(&rxbuffer) == 16);
  assert(strncmp(buf1, str, 16) == 0);

  assert(Q_Enqueue(&rxbuffer,str+32, 32) == 32);  // (1)
  assert(Q_Size(&rxbuffer) == 48);
  assert(Q_Enqueue(&rxbuffer,str+64, cap-64) == cap-64);  // (2)
  assert(Q_Size(&rxbuffer) == cap-16);
  assert(Q_Dequeue(&rxbuffer,buf1+16, cap-16) == cap-16);
  assert(strncmp(buf1, str, cap) == 0);

  assert(Q_Enqueue(&rxbuffer,str, 32) == 32);  // advance so that read < write
  assert(Q_Size(&rxbuffer) == 32);
  assert(Q_Dequeue(&rxbuffer,buf1, 16) == 16);
  assert(Q_Size(&rxbuffer) == 16);
  assert(strncmp(buf1, str, 16) == 0);

  assert(Q_Enqueue(&rxbuffer,str+32, cap-20) == cap-20);  // (3)
  assert(Q_Size(&rxbuffer) == cap-4);
  assert(Q_Dequeue(&rxbuffer,buf1, cap-8) == cap-8);
  assert(strncmp(buf1, str+16, cap-8) == 0);
  assert(Q_Size(&rxbuffer) == 4);
  assert(Q_Dequeue(&rxbuffer,buf1, 8) == 4);
  assert(strncmp(buf1, str+16+cap-8, 4) == 0);
  assert(Q_Size(&rxbuffer) == 0);

  assert(Q_Enqueue(&rxbuffer,str, 49) == 49);  // advance so that read < write
  assert(Q_Size(&rxbuffer) == 49);
  assert(Q_Dequeue(&rxbuffer,buf1, 16) == 16);
  assert(Q_Size(&rxbuffer) == 33);
  assert(strncmp(buf1, str, 16) == 0);

  printf("Passed all the test cases for CBFIFO for UART functionality\n\r");
  printf("-----------------------------------------------------------\n\r\n\r");
  return PASS;
}




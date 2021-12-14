/**
* @file test_cbfifo.h
* @brief Header file for test_cbfifo.c source file
*
* This header file consists of function prototypes used in test_cbfifo.c
* @date 10th November, 2021
* @author Shreyan Dhananjayan (shdh7090@colorado.edu)
* @Tools   MCU Expresso IDE, KL25Z Freedom development board
*/

#include"queue.h"

/*Function Prototypes*/

/**
* @brief Function to execute the cbfifo test functions which
* is called from main function
*
* @return 1 if test functions are executed properly 
* else zero if there is an error in executing the test functions
*/

bool test_cbfifo();

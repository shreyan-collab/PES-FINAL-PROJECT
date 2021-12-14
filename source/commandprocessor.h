   /**
 * @file    commandprocessor.h
 * @brief   This source file consists of functions prototypes of the command processor
 * 			state machine
 * @date 	10th December, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 * 1) Howdy Presentation 22 Command Processing
 * 2) Embedded Systems fundamentals with ARM Cortex-M based microcontroller by Alexander Dean
 */

#ifndef COMMANDPROCESSOR_H_
#define COMMANDPROCESSOR_H_




/*
 * @brief Handler function for set angle command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_set_angle(int argc, char *argv[]);
/*
 * @brief Handler function for calibrate command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_calibrate(int argc, char *argv[]);
/*
 * @brief To accumulate the input buffer and handles user backspace
 *
 * @return void
 */
void accumulator();

/*
 * @brief Parses the input string in to tokens and calls the handler function
 *
 * @param1 the charatcer pointer to input string

 * @return void
 */
void process_command(char *input);

/*
 * @brief Handler function for author command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_author(int argc, char *argv[]);


/*
 * @brief Handler function for info command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_info(int argc, char *argv[]);


/*
 * @brief Handler function for help command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_help(int argc, char *argv[]);


#endif /* COMMANDPROCESSOR_H_ */


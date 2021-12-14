/**
 * @file    commandprocessor.c
 * @brief   This source file consists of functions definitions of the command processor
 * 			state machine
 * @date 	10th Decemberr, 2021
 * @author 	Shreyan Prabhu
 * @Tools   MCU Expresso IDE, KL25Z Freedom development board
 * @References
 * 1) Howdy Presentation 22 Command Processing
 * 2) Embedded Systems fundamentals with ARM Cortex-M based microcontroller by Alexander Dean
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "commandprocessor.h"
#include "switch.h"
#include "LEDs.h"
#include "accelerometer.h"

#define MINIMUM_ANGLE 0				/*Maximum and minimum angle that can be shared*/
#define MAXIMUM_ANGLE 180
#define DEGREE_90      90
#define MAX_INTENSITY 0xFF
#define GREEN 0xFF
#define RED   0xFF
#define BLUE  0xFF
#define OFF 	 0

typedef void (*command_handler_t)(int, char *argv[]);

int reference = 0;
int maximum_angle = 180;
typedef struct
 {
  const char *name;
  command_handler_t handler;
  const char *help_string;
} command_table_t;


static const command_table_t commands[] ={{"author",handle_author,"1. Type <Author>(case insensitive) to know the author's name \n\r"},
										  {"calibrate",handle_calibrate,"2. Type <calibrate> to set a reference position as 0 with respect to which angle wll be measured\n\r"},
										  {"help",handle_help,"3. Type <help>(case insensitive) to know about the possible commands\n\r"},
										  {"info",handle_info,"4. Type <info>(case insensitive) to know about the build information\n\r"},
										  {"set", handle_set_angle,"5. Type <set> followed by <angle> to measure angle with respect to the reference position you have given\n\r"}};



static const int num_commands = sizeof(commands) / sizeof(command_table_t);


char buffer1[200];   /*Buffer used to take input from the character*/
int i=0;			 /*To store the buffer index*/


/*
 * @brief Handler function for help command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_help(int argc, char *argv[])
{
	if(argc!=1)
	{
		printf("Wrong Syntax! Refer Help for Help syntax\n\r"); /*Checking the number of tokens*/
		return;
	}
	for(int i=0;i<num_commands;i++)
	{
		printf(commands[i].help_string);
	}

}

/*
 * @brief Handler function for calibrate command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_calibrate(int argc, char *argv[])
{
	if(argc!=1)
	{
		printf("Wrong Syntax! Refer Help for calibrate syntax\n\r");
		return;
	}
	update_led_colour(RED, OFF, OFF);
	printf("Move the board to reference zero and press switch to set the position\n\r");
	reset_switch();
	while(! check_switch_pressed());
	reference = abs(get_roll());
	if (reference < DEGREE_90)										/*Checking whether reference is above or below 90*/
	{
		maximum_angle =  MAXIMUM_ANGLE - reference;					/*maximum angle that can be measured post zero reference*/
		printf("\n\rSwitch is pressed, Reference zero angle is set as %d, Use this to measure the angle you require\n\r", reference);
		printf("\n\rWith this zero reference , you can measure up to %d\n\r",maximum_angle);
		update_led_colour(OFF, GREEN, OFF);
	}
	else if (reference > DEGREE_90)
	{
		printf("The reference zero can only be set between 0 to 90, type calibrate to again set zero reference\n\r");
		return;
	}

}

/*
 * @brief Handler function for set angle command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_set_angle(int argc, char *argv[])
{
	int input_angle=0;								/*The angle given by user*/
	int measure_angle=0;
	int angle_zero=0;
	float intensity_red=0;							/*To store the intensity of Red , Blue, Green to be loaded in TPM registers*/
	float intensity_green=0;
	float intensity_blue =0;
	if(argc!=2)
	{
		printf("Wrong Syntax! Refer Help for set(angle) syntax\n\r");
		return;
	}
	for(int i=0;argv[1][i]!='\0';i++)
	{
		if((argv[1][i] > '9') || (argv[1][i] < '0'))		/*Checking whether alphabets present in length without 0x*/
		{
			printf("Enter Valid Angle from 0 - 180\n\r");
			return;
		}
	}
	input_angle =  strtol(argv[1],NULL, 10);							/*Converting the address to decimal representation*/
	if ((input_angle < MINIMUM_ANGLE) || (input_angle > MAXIMUM_ANGLE))
	{
		printf("Enter Valid angle from 0 - 180\n\r");
		return;
	}
	else if(maximum_angle < input_angle)
	{
		printf("The maximum angle that can be measured is %d\n\r", maximum_angle);
		return;

	}
	else
	{
		printf("Input angle given as %d, Move the accelerometer to the desired angle\n\r", input_angle);
	}
	update_led_colour(OFF, OFF, GREEN);
	while (measure_angle != input_angle)
	{
		angle_zero = abs(get_roll());
		measure_angle = abs(get_roll()) - reference ;
		if (measure_angle == input_angle)
		{
			update_led_colour(RED, OFF, OFF);
		}
		else if((angle_zero <= reference) && (reference !=0))				/*When angle is far away from the destination angle*/
		{
			intensity_green = (((float)angle_zero / (float) reference) * MAX_INTENSITY); /*Green value decrease when angle is far away*/
			intensity_blue = (((float)(reference - angle_zero) / (float) reference ) * MAX_INTENSITY);
			update_led_colour(OFF, intensity_green, intensity_blue);
		}
		else if((measure_angle <= reference) || (reference == 0))		/*When angle is close to the destination angle*/
		{
			intensity_red = (((float)measure_angle / (float)input_angle) * MAX_INTENSITY); /*Red value increases when angle is closer*/
			intensity_green = (((float)(input_angle - measure_angle) / (float)input_angle ) * MAX_INTENSITY);
			update_led_colour(intensity_red, intensity_green, OFF);
		}

	}
	printf("Desired angle is reached\n\r");
	reference = 0;
	printf("Type calibrate if you want to set reference position before measuring another angle\n\r");
	printf("Current Reference Angle = %d\n\r",reference);


}

/*
 * @brief Handler function for author command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_author(int argc, char *argv[])
{
	if(argc!=1)
	{
		printf("Wrong Syntax! Refer Help for author syntax\n\r");
		return;
	}
	printf("Shreyan\n\r");
}

/*
 * @brief Handler function for info command
 *
 * @param1 argc number of tokens
 * @param2 argv Every index consists a token
 * @return void
 */
void handle_info(int argc, char *argv[])
{
	if(argc!=1)
	{
		printf("Wrong Syntax! Refer Help for info syntax\n\r");
		return;
	}
	printf("Current Roll Angle: %d\n\r",abs(get_roll()));
	//printf("Version Tag:%s -- Build Machine:%s -- Build Date: %s\n\r",VERSION_TAG, VERSION_BUILD_MACHINE,VERSION_BUILD_DATE);
}


/*
 * @brief Parses the input string in to tokens and calls the handler function
 *
 * @param1 the charatcer pointer to input string

 * @return void
 */
void process_command(char *input)
{

  int commandCount=0;
  char *p = input;
  char *end;
  for (end = input; *end != '\0'; end++);		 /* To Find End of String*/

  bool in_token = false;						 /*Parse the string in to tokens*/
  char *argv[10];
  int argc = 0;
  memset(argv, 0, sizeof(argv));
  for (p = input; p < end; p++)
  {
	 if((*p == ' ' || *p == '\t') && in_token == true)
	 {
		 *p = '\0';
		 in_token = false;
	 }
	 else if(*p !=' ' && *p!= '\t' && in_token == false)
	 {
		 argv[argc] = p;
		 in_token = true;
		 argc++;
	 }

  }

  argv[argc] = NULL;
  if (argc == 0)   										/*When no command is given as input*/
    return;

  for ( commandCount=0; commandCount < num_commands; commandCount++)
  {
      if (strcasecmp(argv[0], commands[commandCount].name) == 0)
      {
        commands[commandCount].handler(argc, argv);
        break;
      }
  }

  if(commandCount == (num_commands))
  {
	  printf("Unknown Command:%s\n\r",argv[0]);
  }
}


/*
 * @brief To accumulate the input buffer and handles user backspace
 *
 * @return void
 */
void accumulator()
{

    int count=0;										/*Variable used for backspace functionality*/
	while(1)
	{
	buffer1[i] = getchar();								/*Getting input from the user*/
	if (buffer1[i] == '\b')
	{
		if(count > 0)
		{
		printf("%c",buffer1[i]);						/*Echoing the character to the user*/
		count--;
		printf(" \b \b");
		i=i-2;										/*Decrementing by 2 to include backspace and the character removed*/
		}
	}
	else
	{
		printf("%c",buffer1[i]);
		count++;
	}

	if(buffer1[i] == '\r')					    	/*Check when the entire command is complete*/
	{
	printf("\n\r");
	buffer1[i]='\0';
	process_command(buffer1);
	i=0;
	break;
	}
	else
	{
		i++;
	}
	}

}




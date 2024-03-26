/****************************************************************************
Author: Conor Carrigan
Lab 7: Input and output capture
Date Created: March 6, 2024
Last Modified:March 6, 2024
Description: this program will measure reaction time using leds and switches and the input output compare
Inputs: switch push
Outputs: Reaction time from when led turned on
****************************************************************************/

#include "TimerDrivers.h"
#include "wavegen.h"
#include "LCD.h"
#include <stdio.h>// include for int to char func

extern unsigned char updateflag;
extern unsigned long time;

void intToChar(int num, char *str) {
  // Convert the integer to a string
  sprintf(str, "%d", num);
}
int main(void)
{
	//part 1 main
	uint32_t period = 25000;
	PF4_Init() ;
	PWM_Init(&TogglePF4, period);
	// part 2 main
	int display_time;
	char m = 'm' ;
	char s = 's';
	char str[2];
	Timer0A_Init();
	Timer1A_Init();
	PortFInit();
	updateflag = 0; // set update and time to zero
	time = 0;
    while (1){
		//GPIOF->DATA |= 0x04; // manually test led on
		if (updateflag == 1)
			// if theres a sw2 input then do this 
			display_time = time * .02 ;// get time in ms
			// display to LCD
			intToChar(display_time,str);
			LCD_command(0x08);
			LCD_data(str[0]) ; // diapay time in ms
			LCD_command(0x08+1);
			LCD_data(str[1]);
			LCD_command(0x08+2);
			LCD_data(m);
			LCD_command(0x08+3);
			LCD_data(s); // display ms after displaying time
		
			updateflag = 0;

	}
	
}
/****************************************************************************
Author: Logan Tjossem
Lab 7 part b
Date Created: March 6, 2024
Last Modified:
Description: This program will measure reaction time and display it on the LCD 
			 display using the format: xxxx ms. 
****************************************************************************/

#include "TM4C123GH6PM.h"
#include "timers.h"
#include "LCD.h"

int time = 0;
int updateFlag = 0;

// Initialize Ports
void Ports_Init(void)
{
    SYSCTL->RCGCGPIO |=0x21;
    while ((SYSCTL->RCGCGPIO&0x21)==0){}
	GPIOF->LOCK = 0X4C4F434B;
	GPIOF->CR = 0x01;
	GPIOF->PUR |= 0x11;
    GPIOF->DIR|=0x04; // LED IS PF2
	GPIOF->DIR&=~0x11; // SW1 is PF4, SW2 is PF0
    GPIOF->DEN|=0x15;
}


void TIMER0A_Handler(void){
	TIMER0->ICR = 0x00000001; // acknowledge timer0A timeout
	GPIOF->DATA |= 0x04;
	TIMER1->IMR |= 0x00000004;
	TIMER1->CTL |= 0x00000001;
	TIMER0->IMR &= 0x00000004;
	TIMER0->CTL &= ~0x00000001;
	time = TIMER1->TAR;
}

void TIMER1A_Handler(void){
	TIMER1->ICR = 0x00000001; // acknowledge timer1A timeout
	time = (time-TIMER1->TAR)&0x00FFFFFF;
	GPIOF->DATA &= ~0x04;
	updateFlag = 1;
	TIMER1->IMR &= 0x00000004;
	TIMER1->CTL &= 0x00000001;
	TIMER0->IMR |= 0x00000004;
	TIMER0->CTL |= ~0x00000001;
}

int main(void) {
	Ports_Init();
	Timer0A_Init();
	Timer1A_Init();
	LCD_Init();
	while(1) {
		if (updateFlag==1) {
			int realtime = time *0.2;
			LCD_command(0x80);
			LCD_data(realtime);
			LCD_Str("ms");
			updateFlag=0;
		}
	}
	return 0;
}

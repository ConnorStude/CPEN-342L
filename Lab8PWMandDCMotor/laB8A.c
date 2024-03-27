// c driver for pmw Dc motors 
#include "lab8.h"
void PWM3B_Init(uint16_t period, uint16_t high) {
SYSCTL->RCGCTIMER |= 0x08;
SYSCTL->RCGCGPIO |= 0x02;
while ((SYSCTL->RCGCGPIO &0x02)==0){}
	GPIOB-> AFSEL |=0x08;
	GPIOB ->DEN |=0x08;
	GPIOB->PCTL = (GPIOB->PCTL &0xFFFF0FFF)+0x00007000;
	TIMER3->CTL &=~0x00000100;
	TIMER3->CFG = 0x00000004;
	TIMER3-> TBMR = 0x0000000A;
	TIMER3->TBILR= period-1;
	TIMER3->TBMATCHR = period-high-1; ;
	TIMER3-> CTL |=0x0000000100;}

void PWM3B_Duty(uint16_t high) {
	TIMER3->TBMATCHR = TIMER3->TBILR-high ;}

//ignore this file
#include "TM4C123GH6PM.h"

void PWM3B_Init(uint16_t period, uint16_t high){
	SYSCTL->RCGCTIMER |=0x08;
	SYSCTL->RCGCGPIO|=0x02;
	while((SYSCTL->RCGCGPIO &0x02)==0){}
	GPIOB->AFSEL|=0x08;
	GPIOB->DEN |=0x08;
	GPIOB->PCTL=(GPIOB->PCTL &0xFFFF0FFF)+0x00007000;
	TIMER3->CTL &=~0x00000100;
	TIMER3->CFG=0x00000004;
	TIMER3->TAMR=0x0000000A;
	TIMER3->TAILR=period-1;
	TIMER3->TAMATCHR=period-high-1;
	TIMER3->CTL|=0x00000100;
	}

void PWM3B_Duty(uint16_t high){
	TIMER3->TAMATCHR=TIMER3->TAILR-high;
}


int main(void){
	PWM3B_Init(1000,20000);
	PWM3B_Duty(35000);
}
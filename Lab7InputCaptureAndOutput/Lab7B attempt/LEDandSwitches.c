#include "TM4C123GH6PM.h"
#include "LEDandSwitches.h"

void LEDSwitchPortInit(void)
{
    SYSCTL->RCGCGPIO |=0x20; //enable clock for PF
    while((SYSCTL->RCGCGPIO&0x20)==0){}
    GPIOF->LOCK =0X4C4F434B;
		GPIOF->CR=0xFF;
		GPIOF->DIR |= 0x04;         
		GPIOF->DIR &= ~0x11;  
		GPIOF->PUR |=0x11;			
    GPIOF->DEN |= 0x15;
		
}

void SwitchInit(void)
{
	  SYSCTL->RCGCGPIO |=0x20; 
    while((SYSCTL->RCGCGPIO&0x20)==0){}
    GPIOF->DIR &= ~0x1E;         
    GPIOF->DEN &= ~0x1E; 	
}

void turnOn (void)
{
		GPIOF->DATA |= 0x04;	
}

void turnOff (void)
{
		GPIOF->DATA &= ~0x04;
}
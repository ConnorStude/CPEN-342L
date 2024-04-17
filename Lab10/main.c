#include "TM4C123GH6PM.h"
#include <stdio.h>

//this is too listen for the change in output
void PA_init(void){
		SYSCTL->RCGCGPIO |=0x01;
    while((SYSCTL->RCGCGPIO&0x01)==0){}    
    GPIOA->DIR &= ~0x80;        
    GPIOA->DEN |= 0x80;   
}

void SSI1DAC_Init(void){
	SYSCTL->RCGCSSI|=2;
	SYSCTL->RCGCGPIO|=0x08;
	while ((SYSCTL->PRGPIO&0x08)==0){};
	GPIOD->DEN|=0x0B;
	GPIOD->AFSEL |=0x09;
	GPIOD->PCTL&=~0x0000F00F;
	GPIOD->PCTL|=0x00002002;
	GPIOD->DIR|=0x02;
	SSI1->CR1=0;
	SSI1->CPSR = 10; /* prescaler divided by 10 */
	SSI1->CR0=0x0047;
	SSI1->CR1|=2;
}

void SSI1DAC_Write(unsigned char data){
	while((SSI1->SR&2)==0){}
	SSI1->DR=data;
	while(SSI1->SR&0x10);
}

int main(void){
  SSI1DAC_Init();
	PA_init();
	volatile unsigned char highDgt=0, lowDgt=0;
	volatile uint16_t resistorValue;
	uint16_t index = 1;
	while (1){	
		GPIOD->DATA |= 0x02;
		GPIOD->DATA &= ~0x02;	
		lowDgt=((unsigned char)index);
		highDgt=((unsigned char)(index>>8));
		SSI1DAC_Write(highDgt);
		SSI1DAC_Write(lowDgt);
		index++;//increases index voltage level
		if ((GPIOA->DATA&0x80)==0){
			resistorValue=(2*230*4096)/(5*index);//gets the resistor value from the equation
			index=0;
		}
	}
}

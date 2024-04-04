#include "TM4C123GH6PM.h"

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
	while(1){
		volatile unsigned char highDgt=0, lowDgt=0;
		for (int i =0;i<4095;i++)
		{
			GPIOD->DATA |= 0x02;
			GPIOD->DATA &= ~0x02;
			lowDgt = (unsigned char)i;
			highDgt = (unsigned char)(i>>8);
			SSI1DAC_Write(highDgt);
			SSI1DAC_Write(lowDgt);
		}
		for (int j =4095;j>0;j--)
		{
			GPIOD->DATA |= 0x02;
			GPIOD->DATA &= ~0x02;
			lowDgt = (unsigned char)j;
			highDgt = (unsigned char)(j>>8);
			SSI1DAC_Write(highDgt);
			SSI1DAC_Write(lowDgt);
		}
	}
}

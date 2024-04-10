#include "TM4C123GH6PM.h"
#include <stdio.h>
#include "LCD.h"

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

void hex2dec(unsigned char x,char *str)
{
	sprintf(str,"%d",x);
	LCD_Str(str);//sends to LCD
	LCD_data('V');
}

unsigned char DIPRead(){	
	return (GPIOF->DATA&0x1E)>>1;
}	


void DIPinit(){
	SYSCTL->RCGCGPIO|=0x20;
	while ((SYSCTL->RCGCGPIO&0x20)==0){}
	GPIOF->DEN|=0x1E;	
	GPIOF->DIR&=~0x1E;
}

int main(void){
	LCD_init();
	DIPinit();
  SSI1DAC_Init();
	unsigned char input;
	char* result;
	volatile uint16_t num;
	volatile unsigned int value = 4095;
	volatile unsigned char highDgt=0, lowDgt=0;
	while (1)
	{	
		input=DIPRead();
		hex2dec(input,result);//converts to dec and sends to LCD
		num=input;
		GPIOD->DATA |= 0x02;
		GPIOD->DATA &= ~0x02;	
		num = value*num/15;
		lowDgt=((unsigned char)num);
		highDgt=((unsigned char)(num>>8));
		SSI1DAC_Write(highDgt);
		SSI1DAC_Write(lowDgt);
		delayMs(100);
		LCD_command(0x01);//clear display	
	}
}

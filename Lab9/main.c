/****************************************************************************
Author: Joel George Vincent
Lab 9 – DA Applications
Date Created: April 6, 2024
Last Modified: April 6, 2024
Description: Use SSI and a TLV5616 D/A converter to generate a triangular waveform with -5V ~ +5V
peak-to-peak value. The voltage range shall be 0 to 15V. Use the DIP switch to enter the
voltage 0-15 V that will be generated. The generated voltage shall be displayed on the LCD
using the format “xx V”.
Inputs: DIP
Outputs: LCD, oscilliscope
****************************************************************************/
//#include "SSIDAC.h"
#include "TM4C123GH6PM.h"

void SSI1DAC_Init(void);
void SSI1DAC_Write(unsigned char data);

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


void SSI1DAC_Init(void) {
	SYSCTL->RCGCSSI |= 2; /* enable clock to SSI1 */
	SYSCTL->RCGCGPIO |= 0x08; /* enable clock to GPIOD for SSI1*/
	while ((SYSCTL->PRGPIO & 0x008) == 0) {}; // ready?
	GPIOD->DEN |= 0x0B; /* make PD0,1,3 digital */
	GPIOD->AFSEL |= 0x09; /* enable alternate function for pin0,3 */
	GPIOD->PCTL &= ~0x0000F00F; /* assign pins to SSI1 */
	GPIOD->PCTL |= 0x00002002; /* assign pins to SSI1 */
	GPIOD->DIR |= 0x02; // set pin 1 as output
	SSI1->CR1 = 0; /* disable SSI and make it master */
	SSI1->CPSR = 10; /* prescaler divided by 10 */
	SSI1->CR0 = 0x0047; /* 5 MHz SSI clock, SPI mode, 8 bit data */
	SSI1->CR1 |= 2; /* enable SSI1 */
}

void SSI1DAC_Write(unsigned char data) {
	while ((SSI1->SR & 2) == 0) {}; /* wait until FIFO not full */
	SSI1->DR = data; /* transmit high byte */
	while (SSI1->SR & 0x10); /* wait until transmit complete */
}

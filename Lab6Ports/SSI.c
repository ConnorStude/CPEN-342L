#include "SSI.h"
#include "TM4C123GH6PM.h"

void SSI1_Init(void) {
	SYSCTL->RCGCSSI |= 0x02; //Activate SSI1 - bit 0 for SSI0, 1 for SSI1 ...
	SYSCTL->RCGCGPIO |= 0x18; // enable clock to Port D and E
	while((SYSCTL->PRGPIO&0x18) == 0) {}; // ready?
		GPIOD->AFSEL |= 0x0F; /* enable alternate function on PD0-PD3*/
	GPIOD->PCTL = (GPIOD->PCTL&0xFFFF0000)+0x00002222;
	GPIOD->AMSEL &= ~0x0F; /* disable analog for these pins */
	GPIOD->DEN |= 0x0F; /* enable digital I/O on PD0-PD3 */
	GPIOE->DEN |= 0x08; // PE3 enable
	GPIOE->DIR |= 0x08; // PE3 output
	GPIOE->DATA |= 0x08; // PE3 default high
	SSI1->CR1 = 0x00; /* disable SSI and make it master */
	SSI1->CPSR = 0x1A; /* 8MHz SSICLK; assume system clock 16MHz */
	SSI1->CR0 &= ~(0x0000FFF0); // SSI, SPO = 0, SPH = 0; SCR=0
	SSI1->CR0 = (SSI1->CR0 &~0x0F) + 0x07; /* 8 bit data */
	SSI1->CR1 |= 0x02; /* enable SSI1 */
}

void SSI1_Write(unsigned char data) // output port expander function
{
	while((SSI1->SR & 2) == 0) {}; /* wait until FIFO not full */
		SSI1->DR = data; /* transmit a byte */
	while(SSI1->SR & 0x10) {}; /* wait until transmit complete */
}

unsigned char SSI1_Read(void) // input port expander function
{
	GPIOE->DATA &= ~0x08;
	GPIOE->DATA |= 0x08; //set to high
	while((SSI1->SR & 0x01) == 0) {};/* wait until FIFO empty */
	SSI1->DR = 0; /* trigger 8 SCK pulses to shift in data */
	while((SSI1->SR & 0x04) == 0) {}; /* wait until FIFO not empty */	
	return SSI1->DR;
}
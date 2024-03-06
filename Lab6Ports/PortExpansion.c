// c file for port expander 
#include "PortExpansion.h
#include "TM4C123GH6PM.h"


void SSI0_Init(void) {
	SYSCTL->RCGCSSI |= 0x02; //Activate SSI0 - bit 0 for SSI0, 1 for SSI1 ...
	SYSCTL->RCGCGPIO |= 0x12; /* enable clock to Port B and E */
	while((SYSCTL->PRGPIO&0x11) == 0) {}; // ready?
		GPIOB->AFSEL |= 0x0F; /* enable alternate function on PA2,3,5*/
	GPIOB->PCTL = (GPIOA->PCTL&0xFF0F00FF)+0x00202200;
	GPIOB->AMSEL &= ~0x0F; /* disable analog for these pins */
	GPIOB->DEN |= 0x0F; /* enable digital I/O on PB 0-3*/
	GPIOE->DEN |= 0x08; /* enable digital I/O on PE 3*/
	SSI1->CR1 = 0x00; /* disable SSI and make it master */
	SSI1->CPSR = 0x0A; /* 5MHz SSICLK; assume system clock 50MHz */
	SSI1->CR0 &= ~(0x0000FFF0); // SSI, SPO = 0, SPH = 0; SCR=0
	SSI1->CR0 = (SSI0->CR0 &~0x0F) + 0x07; /* 8 bit data */
	SSI1->CR1 |= 0x02; /* enable SSI0 */
}

void SSI0_Write(unsigned char data) // output port expander function
{
	while((SSI0->SR & 2) == 0) {}; /* wait until FIFO not full */
		SSI0->DR = data; /* transmit a byte */
	while(SSI0->SR & 0x10) {}; /* wait until transmit complete */
}

unsigned char SSI0_Read(void) // input port expander function
{
	GPIOE->DATA &= ~0x08;
	GPIOE->DATA |= 0x08;
	while((SSI0->SR & 0x01) == 0) {};/* wait until FIFO empty */
		SSI0->DR = 0; /* trigger 8 SCK pulses to shift in data */
	while((SSI0->SR & 0x04) == 0) {}; /* wait until FIFO not empty */
	return SSI0->DR;
}

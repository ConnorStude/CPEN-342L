// c file for port expander 
#include "PortExpansion.h
#include "TM4C123GH6PM.h"


#include "TM4C123GH6PM.h"
void SSI0_Init(void) {
	SYSCTL->RCGCSSI |= 0x01; //Activate SSI0 - bit 0 for SSI0, 1 for SSI1 ...
	SYSCTL->RCGCGPIO |= 0x08; /* enable clock to Port A */
	while((SYSCTL->PRGPIO&0x08) == 0) {}; // ready?
		GPIOA->AFSEL |= 0x0F; /* enable alternate function on PA2,3,5*/
	GPIOA->PCTL = (GPIOA->PCTL&0xFF0F00FF)+0x00202200;
	GPIOA->AMSEL &= ~0x0F; /* disable analog for these pins */
	GPIOA->DEN |= 0x0F; /* enable digital I/O on PA2,3,5 */
	SSI0->CR1 = 0x00; /* disable SSI and make it master */
	SSI0->CPSR = 0x1A; /* 8MHz SSICLK; assume system clock 16MHz */
	SSI0->CR0 &= ~(0x0000FFF0); // SSI, SPO = 0, SPH = 0; SCR=0
	SSI0->CR0 = (SSI0->CR0 &~0x0F) + 0x07; /* 8 bit data */
	SSI0->CR1 |= 0x02; /* enable SSI0 */
}

void SSI0_Write(unsigned char data) // output port expander function
{
	while((SSI0->SR & 2) == 0) {}; /* wait until FIFO not full */
		SSI0->DR = data; /* transmit a byte */
	while(SSI0->SR & 0x10) {}; /* wait until transmit complete */
}

unsigned char SSI0_Read(void) // input port expander function
{
	while((SSI0->SR & 0x01) == 0) {};/* wait until FIFO empty */
		SSI0->DR = 0; /* trigger 8 SCK pulses to shift in data */
	while((SSI0->SR & 0x04) == 0) {}; /* wait until FIFO not empty */
	return SSI0->DR;
}

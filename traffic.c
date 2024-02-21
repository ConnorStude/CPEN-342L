// PRELAB
#include "traffic.h"
#include "TM4C123GH6PM.h"

int count = 0;

void traffic_init(void)
{
    SYSCTL->RCGCGPIO |= 0x02;   // enable clock to GPIOF & D
    while ((SYSCTL->RCGCGPIO&0x02)==0){}
    GPIOD->DIR |= 0x3F;
    GPIOD->DEN |= 0x3F;
}

void SysTick_Init(void)
{
    SysTick->CTRL = 0; // 1) disable SysTick during setup
    SysTick->LOAD = 0x00FFFFFF; // 2) maximum reload value
    SysTick->VAL = 0; // 3) any write to CURRENT clears it
    SysTick->CTRL = 0x00000005; // 4) enable SysTick with core clock
	
	__enable_irq();
}

void PLL_init(void)
{
    SYSCTL->RCC2 |= 0x80000000;
    SYSCTL->RCC2 |= 0x00000800; // bypass PLL while initializing
    SYSCTL->RCC=(SYSCTL->RCC&~0x000007C0)+0x00000540; // 16 MHz
    SYSCTL->RCC2&=~0x00000070; // configure for main oscillator source
    SYSCTL->RCC2&=~0x00002000; // activate PLL
    SYSCTL->RCC2|=0x40000000; // use 400MHz PLL
    SYSCTL->RCC2=(SYSCTL->RCC2&~0x1FC0000)+(39<<22); // 400/(39+1) = 10MHz
    while ((SYSCTL->RIS&0x00000040)==0){};
    SYSCTL->RCC2&=~0x00000800; // enable PLL
}

void SysTick_Handler(void) {
	count++;
	switch (count) {
		case 1:
			Green_Red();
			break;
		case 11:
			Yellow_Red();
			break;
		case 13:
			Red_Red();
			break;
		case 14:
			Red_Green();
			break;
		case 24:
			Red_Yellow();
			break;
		case 26:
			Red_Red();
			count = 0;
			break;
	}
}

// 10 seconds
void Red_Green(void)
{
    GPIOB->DATA = 0x11;
}

// 2 seconds
void Red_Yellow(void)
{
    GPIOB->DATA = 0x21;
}

// 10 seconds
void Green_Red(void)
{
    GPIOB->DATA = 0x0A;
}

// 2 seconds
void Yellow_Red(void)
{
    GPIOB->DATA = 0x0C;
}

// 1 second
void Red_Red(void)
{
    GPIOB->DATA = 0x09;
}

#include "traffic_lights.h"

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
}

void PLL_init(void)
{
    SYSCTL->RCC2 |= 0x80000000;
    SYSCTL->RCC2 |= 0x00000800;
    SYSCTL->RCC=(SYSCTL->RCC~&x0x000007C0)+0x00000540;
    SYSCTL->RCC2 &~0x00000070;
    SYSCTL->RCC2&~0x00002000;
    SYSCTL->RCC2|=0x40000000;
    SYSCTL->RCC2=(SYSCTL->RCC2&~0x1FC0000)+(639<<22);
    while ((SYSCTL->RIS&0x00000040)==0){};
    SYSCTL->RCC2&=~0x00000800;
}

void SysTick_Wait(uint32_t delay)
{
    SysTick->LOAD = delay-1; // number of counts
    SysTick->VAL = 0; // any value written to CURRENT clears
    while((SysTick->CTRL&0x00010000)==0){} // wait for flag
}

//this mode lasts 10 seconds
//input: none
//ouput: none
void Red_Green(void)
{
    GPIOB->DATA = 0x11;
}

//this mode lasts 2 seconds
//input: none
//ouput: none
void Red_Yellow(void)
{
    GPIOB->DATA = 0x21;
}

//this mode lasts 10 seconds
//input: none
//ouput: none
void Green_Red(void)
{
    GPIOB->DATA = 0x0A;
}

//this mode lasts 2 seconds
//input: none
//ouput: none
void Yellow_Red(void)
{
    GPIOB->DATA = 0x0C;
}

//this mode lasts 1 second
//input: none
//output: none
void Red_Red(void)
{
    GPIOB->DATA = 0x09;
}


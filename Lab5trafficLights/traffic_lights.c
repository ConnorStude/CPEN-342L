#include "traffic_lights.h"

//initializes port B for t LED lights
//input: none
//output: none
void traffic_init(void)
{
    SYSCTL->RCGCGPIO |= 0x02;   // enable clock to GPIOF & D
    while ((SYSCTL->RCGCGPIO&0x02)==0){}
    GPIOB->DIR |= 0x3F;
    GPIOB->DEN |= 0x3F;
}

//this mode lasts 10 seconds
//input: none
//ouput: none
void Red_Green(void)
{
    GPIOB->DATA = ~0x11;
}

//this mode lasts 2 seconds
//input: none
//ouput: none
void Red_Yellow(void)
{
    GPIOB->DATA = ~0x21;
}

//this mode lasts 10 seconds
//input: none
//ouput: none
void Green_Red(void)
{
    GPIOB->DATA = ~0x0A;
}

//this mode lasts 2 seconds
//input: none
//ouput: none
void Yellow_Red(void)
{
    GPIOB->DATA = ~0x0C;
}

//this mode lasts 1 second
//input: none
//output: none
void Red_Red(void)
{
    GPIOB->DATA = ~0x09;
}


#include "LED_Display.h"

//PF1-PF4 and PD0-PD3 initialized for output
//inputs: none
//outputs: none
void LED_Display_init(void)
{
    SYSCTL->RCGCGPIO |= 0x28;   // enable clock to GPIOF & D
    while ((SYSCTL->RCGCGPIO&0x28)==0){}
    GPIOD->DIR |= 0x0F;
    GPIOD->DEN |= 0x0F;
    GPIOF->DIR |= 0x1E;
    GPIOF->DEN |= 0x1E;
}

// Function to display a hex digit
void displayHexDigit(unsigned char num[i][j]) 
{
    for (int k=0,k<5,k++)
    {
        GPIOD->DATA |= (num[i][j] | 0x07);
        GPIOF->DATA |= ((num[i][j] | 0xF8) << 1);
        //send data
        j++
    }
}
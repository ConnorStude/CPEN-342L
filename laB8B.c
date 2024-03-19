// c file for MOT12 part of lab 
#include "TM4C123GH6PM.h"
void MOT12_Init(uint16_t period, uint16_t duty){ // motor connects MOT1 & MOT2
SYSCTL->RCGCPWM |= 0x02; // enable clock to PWM1
SYSCTL->RCGCGPIO |= 0x20; // enable clock to GPIOF
SYSCTL->RCGCGPIO |= 0x02; // enable clock to GPIOB
delayMs(1); // PWM1 seems to take a while to start
SYSCTL->RCC &= ~0x00100000; // use system clock for PWM
PWM1->_3_CTL = 0; // disable PWM1_3 during configuration
PWM1->_3_GENB = 0x00000C08; // output low for load, high for match
PWM1->_3_LOAD = period-1;
PWM1->_3_CMPB = duty-1;
PWM1->_3_CTL = 1; // enable PWM1_3
PWM1->ENABLE |= 0x80; // enable PWM1
GPIOF->DIR |= 0x04; // set PF2 pins as output (LED) pin
GPIOF->DEN |= 0x04; // set PF2 pins as digital pins
GPIOF->AFSEL |= 0x04; // enable alternate function
GPIOF->PCTL &= ~0x00000F00; // clear PF2 alternate function
GPIOF->PCTL |= 0x00005000; // set PF3 alternate function to PWM
GPIOB->DEN |= 0x03; // PB0-1 as digital pins
GPIOB->DIR |= 0x03; // set PB0-1 as output
}

void MOT12_Dir_Set(uint16_t dir){
if (dir == 1) // backwards
{
GPIOB->DATA |= 0x04;
GPIOB->DATA &= ~0x08;}
Else // forward {  // forward
GPIOB->DATA &= ~0x04;
GPIOB->DATA |= 0x08;}

}
void MOT12_Speed_Set(uint16_t duty){
PWM1->_3_CMPB = duty-1;
}


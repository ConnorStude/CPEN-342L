#include "TM4C123GH6PM.h"
#include "delayMs.h"

void MOT12_Init(uint16_t period, uint16_t duty){ // motor connects MOT1 & MOT2
	SYSCTL->RCGCPWM |= 0x02; // enable clock to PWM1
	SYSCTL->RCGCGPIO |= 0x20; // enable clock to GPIOF
	SYSCTL->RCGCGPIO |= 0x02; // enable clock to GPIOB
	delayMs(1); // PWM1 seems to take a while to start
	SYSCTL->RCC &= ~0x00100000; // use system clock for PWM
	PWM1->_3_CTL = 0; // disable PWM1_3 during configuration
	PWM1->_3_GENA = 0x000000C8; // output low for load, high for match
	PWM1->_3_LOAD = period-1;
	PWM1->_3_CMPA = duty-1;
	PWM1->_3_CTL = 1; // enable PWM1_3
	PWM1->ENABLE |= 0x40; // enable PWM1M6
	GPIOF->DIR |= 0x04; // set PF2 pins as output (LED) pin
	GPIOF->DEN |= 0x04; // set PF2 pins as digital pins
	GPIOF->AFSEL |= 0x04; // enable alternate function
	GPIOF->PCTL &= ~0x00000F00; // clear PF2 alternate function
	GPIOF->PCTL |= 0x00000500; // set PF2 alternate function to PWM
	GPIOB->DEN |= 0x03; // PB0-1 as digital pins
	GPIOB->DIR |= 0x03; // set PB0-1 as output
}

// function to set direction
void MOT12_Dir(uint16_t dir){
	if (dir == 1) {
		GPIOB->DATA |= 0x01;
		GPIOB->DATA &= ~0x02;
	}
	else {  // forward
		GPIOB->DATA &= ~0x01;
		GPIOB->DATA |= 0x02;
	}
}

// function to set speed
void MOT12_Speed(uint16_t duty) {
	PWM1->_3_CMPA = duty-1; 
}

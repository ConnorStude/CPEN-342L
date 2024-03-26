// c driver for pmw Dc motors 
void PWM3B_Init(uint16_t period, uint16_t high) 
SYSCTL->RCGTIMER |= 0x08;
SYSCTL->RCGGPIO |= 0x02;
While ((SYSCTL->RCGCGPIO &0x02)==0){}
GPIOB-> AFSEL |=0x07;
GPIOB ->den |=0x07;
GPIOB-.PCTL = (GPIOB->PCTL &0xFFFF0FFF)+0x00007000;
Timer3->CTL &=~0x00000001;
Timer3->CFG = 0x00000004;
Timer3-> TAMR = 0x0000000A;
TIMER3->talir= period-1;
TIMER3-. TAMATCHR = period – high -1
TIMER3->CTL |=0x0000000001;}

void PWM3B_Duty(uint16_t high) 
TIMER3->TAMATCHR = TIMER3->Tail-high

//for main for the lab 

PWM3B_Init (10000,)
Void Dip_Read (char dip_val)
If (dipval == 1)
PWM3B_Duty(3300)
Else if (dipval == 2)
PWM3B_Duty(6600)
Else if (dipval == 3)
PWM3B_Duty(9000)
Else 
Return 0;

Part 2:

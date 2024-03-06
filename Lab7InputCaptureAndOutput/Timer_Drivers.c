#define PF4(*((volatile unit32_t*)0x4000543C))
#include "TM4C123GH6PM.h"

void TogglePF4(void){PF4^=0x10;}

void PF4_init(void)
{
//PF4 init
SYSCTL->RCGCGPIO |=0x20;
while((SYSCTL->PRGPIO &0x20)==0){}
GPIOF->DIR |= 0x10;
GPIOF->DEN |= 0x10;
}

void Timer2A_Init(void(*task)(void), uint32_t period)
{
    SYSCTL_RCGCTIMER_R |= 0x04; // 0) activate timer2
    //PeriodicTask = task; // user function
    TIMER2_CTL_R = 0x00000000; // 1) disable timer2A during setup
    TIMER2_CFG_R = 0x00000000; // 2) configure for 32-bit mode
    TIMER2_TAMR_R = 0x00000002; // 3) configure for periodic mode
    TIMER2_TAILR_R = period-1; // 4) reload value
    TIMER2_TAPR_R = 0; // 5) bus clock resolution
    TIMER2_ICR_R = 0x00000001; // 6) clear timer2A timeout flag
    TIMER2_IMR_R = 0x00000001; // 7) arm timeout interrupt
    NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
    NVIC_EN0_R = 1<<23; // 9) enable IRQ 23 in NVIC
    TIMER2_CTL_R = 0x00000001; // 10) enable timer2A
    EnableInterrupts();
}
void Timer2A_Handler(void){
TIMER2_ICR_R = 0x00000001; // acknowledge timer2A timeout
//(*PeriodicTask)(); // execute user task
}

void Timer0A_Init(void)
{
    SYSCTL->RCGCTIMER |= 0x01; // activate timer0
    SYSCTL->RCGCGPIO |= 0x02; // activate port B
    GPIOB->DIR &= ~0x40; // make PB6 input
    GPIOB->AFSEL |= 0x40;// enable alt funct on PB6
    GPIOB->DEN |= 0x40; // configure PB6 as TOCCPO
    GPIOB->PCTL = (GPIOB->PCTL_R&0xF0FFFFFF)+0x07000000;
    TIMERO->CTL &= ~0x00000001; // disable timer0A during setup
    TIMER0->CFG = 0x00000004; // configure for 16-bit capture mode
    TIMERO->TAMR = 0x00000007; // configure for rising edge event
    TIMERO->CTL &= ~0x0000000C; // rising edge
    TIMERO->TAILR = Ox0000FFFF;// start value
    TIMERO->TAPR = 0xFF; // activate prescale, creating 24-bit
    TIMERO->IMR |= 0x00000004; // enable capture match interrupt
    TIMERO->ICR = 0x00000004; // clear timer0A capture match flag
    TIMERO->CTL |= 0x00000001; // timer0A 24-b, +edge, interrupts
    NVIC->IP[19] = 2 << 5; // 19: IRQ number; 2: priority; 5: fixed-shift 5 position
    NVIC->ISER[0] = 1<<19; // enable interrupt 19 in NVIC
    __enable_irq(); /* global enable IRQs */
}

void Timer1A_Init(void)
{
    SYSCTL->RCGCTIMER |= 0x02; // activate timer1
    SYSCTL->RCGCGPIO |= 0x02; // activate port B
    GPIOB->DIR &= ~0x10; // make PB4 input
    GPIOB->AFSEL |= 0x10; // enable alt funct on PB4
    GPIOB->DEN |= 0x10;// configure PB4 as TOCCPO
    GPIOB->PCTL = (GPIOB->PCTL R&0xFOFFFFFF)+0x07000000;
    TIMER1->CTL &= ~0x00000001; // disable timer0A during setup
    TIMER1->CFG = 0x00000004; // configure for 16-bit capture mode
    TIMER1->TAMR = 0x00000007; // configure for rising edge event
    TIMER1->CTL &= ~0x0000000C; // rising edge
    TIMER1->TAILR = Ox0000FFFF; // start value
    TIMER1->TAPR = 0xFF; // activate prescale, creating 24-bit
    TIMER1->IMR |= 0x00000004; // enable capture match interrupt
    TIMER1->ICR = 0x00000004; // clear timer1A capture match flag
    TIMER1->CTL |= 0x00000001; // timer1A 24-b, +edge, interrupts
    NVIC->IP[19] = 2 << 5; // 19: IRQ number; 2: priority; 5: fixed-shift 5 position
    NVIC->ISER[0] = 1<<19; // enable interrupt 19 in NVIC
    __enable_irq(); /* global enable IRQs */
}

void PortA_init(void)
{
    SYSCTL->RCGCGPIO |=0x01;
    while ((SYSCTL->GCGPIO&0x01)==0){}
    GPIOA->DIR|=0x01;
    GPIOA->DEN|=0x01;
}

void PB4_6_init(void)
{
    SYSCTL->RCGCGPIO |=0x02;
    while((SYSCTL->PRGPIO &0x02)==0){}
    GPIOF->DIR &= ~0x50;
    GPIOF->DEN &= ~0x50;
}

int LED_Status(void)
{
    return (GPIOA->DIR & 0x01);
}

int main(void)
{
    PF4_init();
    Timer2A_init(&TogglePF4,80000);
    while (1)
    {}
}
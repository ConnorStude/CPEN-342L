#include "TM4C123GH6PM.h"
#include "LCD.h"
#include "LEDandSwitches.h"
#include "Timer.h"

extern volatile uint32_t time;
extern volatile char updateFlag;

int main(void)
{
	LEDSwitchPortInit();
	LCD_init();
	Timer0A_Init();
	Timer1A_Init();
	while (1)
	{
		if(updateFlag==1)
		{
		updateFlag=0;	
		}
	}
}
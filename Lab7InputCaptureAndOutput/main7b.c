
#include "TM4C123GH6PM.h"
#include "timers.h"
#include "LCD.h"
#include "switch_LED.h"
#include <stdio.h>// include for int to char func

extern volatile uint32_t time;
extern volatile char updateFlag;

void intToChar(int num, char *str) {
  // Convert the integer to a string
  sprintf(str, "%d", num);
}

int main(void) {
	Ports_Init();
	Timer0A_Init();
	Timer1A_Init();
	LCD_init();
	char str[5];
	while(1) {
		if (updateFlag==1) {
			int int_realtime = time / 50000;
			intToChar(int_realtime,str);
			LCD_command(0x01);
			LCD_command(0x80);
			LCD_Str(str);
			LCD_Str("ms");
			updateFlag=0;
		}
	}
	return 0;
}

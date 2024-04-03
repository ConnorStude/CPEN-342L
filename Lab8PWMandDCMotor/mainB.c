#include "TM4C123GH6PM.h"
#include "motor.h"
#include "delay.h"
#include "Keypad.h"
#include "LCD.h"

int main(void) {
	uint16_t period = 4000;
	uint16_t duty = 3000;
	LCD_init();
	MatrixKeypad_Init();
	MOT12_Init(period, duty);	
	LCD_command(0x01);
	while(1) {	
		int32_t num;
		char input = MatrixKeypad_Scan(&num);
		if (input == 'E'){
			LCD_command(0x80);
			LCD_data('+');
			MOT12_Dir_Set(1);
		} else if (input == 'F'){
			LCD_command(0x80);
			LCD_data('-');
			MOT12_Dir_Set(0);
		} else if (input == '0' || input == '1' || input == '2' || input == '3' || input == '4' || input == '5' || input == '6' || input == '7' || input == '8' || input == '9'){
			LCD_command(0x81);
			LCD_data(input);
			uint32_t limit = ((input-0x30)/10)*3999;
			MOT12_Speed_Set(limit);
		}
	}
}

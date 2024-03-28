#include "TM4C123GH6PM.h"
#include "MOTOR.h"
#include "keypad.h"
#include "LCD.h"


int main(void) {
	MOT12_Init(0,0);
	MatrixKeypad_Init();
	LCD_Init();
	char input;
	uint16_t dir = 0;
	uint16_t duty;
	char speed;
	while(1) {
		input = MatrixKeypad_WaitForKeyPress();
		LCD_command(0x01);
		LCD_command(0x80);
		if (input == 0x45) {
			dir = 0;
		} else if (input == 0x46) {
			dir = 1;
		} else if ((input >= 0x30) && (input <= 0x39)) {
			duty = input*70;
			speed = input;
		}
		if (dir == 0) {
			LCD_data(0x2D);
		} else if (dir == 1){
			LCD_data(0x2B);
		}
		LCD_data(speed);
		MOT12_Dir(dir);
		MOT12_Speed(speed);
	}
	
	return 0;
}
/****************************************************************************
Author: Joel George Vincent
Lab 4 - Interface to a 3x5 LED Display
Date Created: February 12, 2024
Last Modified: February 12, 2024
Description: this program will display a hex value between 0 and F
from the last 4 bits of the DIP switch.
Inputs: last 4 bits of the DIP switch.
Outputs: hex values on the Matrix display from 0 to F
****************************************************************************/

#include "TM4C123GH6PM.h"
#include "LCD.h"
#include "delay.h"
#include "DIP.h"
#include "MatrixLED.h"
#include "string.h"
#include <stdbool.h>

int main(void) {
	MatrixLED_Init();
	DIP_Init();
	//LCD_init();
	//LCD_command(0x01);
	
	char DIPval;
	char newVal;
	bool firsttime = true;
	
	while (1) {
		DIPval = Switch_Read();
		if (DIPval >= 0x0A) {
			DIPval += 0x37;
		} else {
			DIPval += 0x30;
		}
		
		
		if (DIPval > 0x39) {
				DisplayMatrixLED(DIPval-17);
		} else {
				DisplayMatrixLED(DIPval);
		}
		/*
		if ((newVal != DIPval)|| firsttime) {
			if (DIPval > 0x39) {
				DisplayMatrixLED(DIPval-17);
			} else {
				DisplayMatrixLED(DIPval);
			}
		}
		newVal = DIPval;
			*/
	}
}

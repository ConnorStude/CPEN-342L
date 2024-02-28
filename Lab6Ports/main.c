#include "TM4C123GH6PM.h"
#include "delay.h"
#include "DIP.h"
#include "SSI_Driver.h"
#include "BAR.h"


int main(void) {
	//Functions
	SSI1_Init();
	DIP_Init();
	BarLED_Init();
	
	while (1) {		
		// Continuously read binary data on Port G
		uint8_t inputData = SSI1_Read();

		// Display the data on the Bar graph LED through Port H
		SSI1_Write(inputData);
	}
}


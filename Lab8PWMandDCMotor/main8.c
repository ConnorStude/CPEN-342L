/****************************************************************************
Author: Conor Carrigan
Lab 8: PWN and DC motor control
Date Created: March 26, 2024
Last Modified:March 26, 2024
Description: This program will control DC motors using PWM 
Inputs: Dip switch and keyboard 
Outputs: dc motors that can change speed and direction 
****************************************************************************/

#include "TM4C123GH6PM.h"
#include "lab8.h"
#include "LCD.h"
#include "PortExpansion.h"
#include "DIP.h"



int main(void) 
{
	PWM3B_Init (10000,3300);
	SSI1_Init() ;
	DIPinit();
	while (1){
	

	char dip_val = (SSI1_Read() & 0x03);
		
	if (dip_val == 0x01)
	PWM3B_Duty(3000);
	else if (dip_val == 0x02)
		PWM3B_Duty(6000);
	else if (dip_val == 0x03)
		PWM3B_Duty(9000);
}}
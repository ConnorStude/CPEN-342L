
#define STEPPER GPIOB->DATA
#include "TM4C123GH6PM.h"
#include "delayMs.h"
#include "keypad.h"
#include "LCD.h"
struct State{
            unsigned char Out; // Output
            const struct State *Next[2]; // CW/CCW
};
char Pos = 0; // between 0 and 47
const struct State *Pt;// Current State
typedef const struct State StateType;
typedef StateType *StatePtr;
#define clockwise 0 // Next index
#define counterclockwise 1 // Next index

StateType fsm[4]={
            { 5,{&fsm[1],&fsm[3]}},
            { 6,{&fsm[2],&fsm[0]}},
            {10,{&fsm[3],&fsm[1]}},
            { 9,{&fsm[0],&fsm[2]}}
};



void SysTick_Init(void){
      SysTick->CTRL = 0; // 1) disable SysTick during setup
      SysTick->LOAD = 0x00FFFFFF; // 2) maximum reload value
      SysTick->VAL = 0; // 3) any write to CURRENT clears it
      SysTick->CTRL = 0x00000005; // 4) enable SysTick with core clock
}

void SysTick_Wait(uint32_t delay){
      SysTick->LOAD = delay-1; // number of counts
      SysTick->VAL = 0; // any value written to CURRENT clears
      while((SysTick->CTRL&0x00010000)==0){ // wait for flag
      }
}

void Stepper_Init(void){
       SysTick_Init();
       SYSCTL->RCGCGPIO |= 0x22; //activate ports B and F
       Pos = 0;
       Pt = &fsm[0];
       GPIOB->DIR |= 0x0F; // make PD3-0 out
       GPIOB->DEN |= 0x0F; // enable digital I/O on PB3-0
       GPIOB->DR8R |= 0x0F; // enable 8 mA drive
	   GPIOF->DIR |= 0x00C; // make PF3 & 2 high
       GPIOF->DEN |= 0x0C; // enable digital I/O on PB3-0
       GPIOF->DATA |= 0x0C; // enable 8 mA drive
}

void Stepper_CW(uint32_t delay){
             Pt = Pt->Next[clockwise]; // circular
             STEPPER = Pt->Out; // step motor
             if(Pos==47){ // shaft angle
                        Pos = 0; // reset
             }
             else{
                        Pos++; // CW
             }
                  SysTick_Wait(delay);
}

void Stepper_CCW(uint32_t delay){
            Pt = Pt->Next[counterclockwise]; // circular
            STEPPER = Pt->Out; // step motor
            if(Pos==0){ // shaft angle
                        Pos = 47; // reset
            }
            else{
                        Pos--; // CCW
            }
            SysTick_Wait(delay);
}

void Stepper_Seek(uint8_t desired, uint32_t time, uint8_t direction) {
            uint16_t steps = desired*16;
            if (direction==0) {
                        while (steps!=0) {
                                    Stepper_CW(time);
                                    steps--;
                        }
            } else {
                        while (steps!=0) {
                                    Stepper_CCW(time);
                                    steps--;
                        }
            }
}


int main(void) {
      Stepper_Init();
      MatrixKeypad_Init();
      LCD_Init();
      char input;
      uint16_t dir = 0;
      uint16_t distance = 0;
	  char distance_display;
      while(1) {
            input = MatrixKeypad_WaitForKeyPress();
            LCD_command(0x01);
            LCD_command(0x80);
            if (input == 0x45) {
                  dir = 0;
            } else if (input == 0x46) {
                  dir = 1;
            } else if ((input >= 0x30) && (input <= 0x39)) {
                  distance_display = input;
				  distance = input-0x30;
				  if (dir == 0) {
					  for (int i=distance*16; i>0; i--) {
								  Stepper_CW(400000);
					  }
				  } else if (dir == 1){
					  for (int i=distance*16; i>0; i--) {
								  Stepper_CCW(400000);
					  }
				}
            }
            if (dir == 0) {
                  LCD_data(0x2D);
            } else if (dir == 1){
                  LCD_data(0x2B);
            }
            LCD_data(distance_display);
}
      
      return 0;
}

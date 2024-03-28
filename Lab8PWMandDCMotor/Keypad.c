#include "Keypad.h"
#include "TM4C123GH6PM.h"
#include "delay.h"

struct Row
{
    uint8_t direction;
    char keycode[4];};
    typedef const struct Row RowType;
    RowType ScanTab[5]={
    { 0x01, "123A" }, // row 0
    { 0x02, "456B" }, // row 1
    { 0x04, "789C" }, // row 2
    { 0x08, "E0FD" }, // row 3
    { 0x00, " " }
};


/* Returns ASCII code for key pressed,
Num is the number of keys pressed
both equal zero if no key pressed */
char MatrixKeypad_Scan(int32_t *Num)
{
    RowType *pt;
    uint32_t column, tmp; char key;
    uint32_t j;
    (*Num) = 0;
    key = 0; // default values
    pt = &ScanTab[0];
    while(pt->direction)
    {
        GPIOD->DIR = pt->direction; // one output
        GPIOD->DATA &= ~0x0F; // DIRn=0, OUTn=HiZ; DIRn=1, OUTn=0
        for(j=1; j<=10; j++); // very short delay
				
        column = ((GPIOA->DATA&0x3C)>>2);// read columns
				delayMs(20);
				tmp = ((GPIOA->DATA&0x3C)>>2);// read columns
				if(column == tmp)
					for(j=0; j<=3; j++)
					{
							if((column&0x01)==0)
							{
									key = pt->keycode[j];
									(*Num)++;
							}
							column>>=1; // shift into position
					}
        pt++;
    }
    return key;
}
//checks if key has been pressed
//input none
//output none
char MatrixKeypad_WaitForKeyPress(void)
{
    int32_t n;
    char letter;
    do {
        letter = MatrixKeypad_Scan(&n);
    } while (n != 1); // Repeat until exactly one key is pressed
    return letter;
}

//checks if key has been released
//input none
//output char value pressed
char MatrixKeypad_WaitForKeyRelease(void)
{
    int32_t n;
    char letter;
    do {
        letter = MatrixKeypad_Scan(&n);
    } while (n != 0); // Repeat until no key is pressed (released)
    return letter;
}

//port init for keypad
//input none
//output none
void MatrixKeypad_Init(void)
{
    SYSCTL->RCGCGPIO |= 0x09; // 1) activate clock for Ports A and D
    while((SYSCTL->RCGCGPIO&0x09) != 0x09){};// ready?
    GPIOA->AFSEL &= ~0x3C; // GPIO function on PA5-2
    GPIOA->AMSEL &= ~0x3C; // disable analog function on PA5-2
    GPIOA->PCTL &= ~0x00FFFF00; // configure PA5-2 as GPIO
    GPIOA->DEN |= 0x3C; // enable digital I/O on PA5-2
    GPIOA->DIR &= ~0x3C; // make PA5-2 in (PA5-2 columns)
    GPIOD->AFSEL &= ~0x0F; // GPIO function on PD3-0
    GPIOD->AMSEL &= ~0x0F; // disable analog function on PD3-0
    GPIOD->PCTL &= ~0x0000FFFF; // configure PD3-0 as GPIO
    GPIOD->DATA &= ~0x0F; // DIRn=0, OUTn=HiZ; DIRn=1, OUTn=0
    GPIOD->DEN |= 0x0F; // enable digital I/O on PD3-0
    GPIOD->DIR &= ~0x0F; // make PD3-0 in (PD3-0 rows)
    GPIOD->DR8R |= 0x0F; // enable 8 mA drive
			
		GPIOA->PUR |=0x3C;	
} 

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "MatrixLED.h"

unsigned char num[16][5] = {
    {0xF7, 0xED, 0xDD, 0xBD, 0x7F}, // '0'
    {0xF1, 0xE9, 0xD9, 0xB9, 0x79}, // '1'
    {0xF7, 0xE9, 0xDF, 0xBC, 0x7F}, // '2'
    {0xF7, 0xE9, 0xDF, 0xB9, 0x7F}, // '3'
    {0xF5, 0xED, 0xDF, 0xB9, 0x79}, // '4'
    {0xF7, 0xEC, 0xDF, 0xB9, 0x7F}, // '5'
    {0xF7, 0xEC, 0xDF, 0xBD, 0x7F}, // '6'
    {0xF7, 0xE9, 0xD9, 0xB9, 0x79}, // '7'
    {0xF7, 0xED, 0xDF, 0xBD, 0x79}, // '8'
    {0xF7, 0xED, 0xDF, 0xB9, 0x79}, // '9'
    {0xF7, 0xED, 0xDF, 0xBD, 0x7D}, // 'A'
    {0xFC, 0xEC, 0xDF, 0xBD, 0x7F}, // 'B'
    {0xF7, 0xEC, 0xDC, 0xBC, 0x7F}, // 'C'
    {0xF1, 0xE9, 0xDF, 0xBD, 0x7F}, // 'D'
    {0xF7, 0xEC, 0xDF, 0xBC, 0x7F}, // 'E'
    {0xF7, 0xEC, 0xDF, 0xBC, 0x7C}, // 'F'
};

void MatrixLED_Init(void) {
    SYSCTL->RCGCGPIO |= 0x28;
    while ((SYSCTL->PRGPIO & 0x28) == 0) {}; //wait until ports D and F are ready

    GPIOD->DIR |= 0x0F;  //Set PD0-PD3 as output
    GPIOD->DEN |= 0x0F;   //enable digital I/O
    // GIPOD->PUR |= 0x0F; pull up resistors prob not needed

    GPIOF->DIR |= 0x1E;  //Set PF1-PF4 as output
    GPIOF->DEN |= 0x1E; //enable digital I/O
}

/*
Pin layout for Matrix LED
.......03..04..10
...14...x...x...x
...08...x...x...x
...05...x...x...x
...01...x...x...x
...07...x...x...x

         PF2->3  PF3->4  PF4->10
PF1->14
PD3->8
PD2->5
PD1->1
PD0->7
*/
void DisplayMatrixLED(char hex) {
    uint8_t decimal;
    if (hex >= '0' && hex <= '9') {
        decimal = hex - '0'; // Convert ASCII digit to its decimal equivalent
    }
    else {
        decimal = hex - 'A' + 10; // Convert ASCII letter to its decimal equivalent
    }

    //unsigned char list[] = num[decimal];

    for (int i = 0; i < sizeof(num[decimal]); i++) {
        uint8_t val = num[decimal][i];
		uint8_t store = ((val & 0x0F) << 1)^0x1E;
        GPIOF->DATA = store;
		uint8_t shift = ((val >> 4) & 0x0F)^0x0F;
        GPIOD->DATA = shift; //shift the val to the right by 4, and then to the left by 1
    }

}


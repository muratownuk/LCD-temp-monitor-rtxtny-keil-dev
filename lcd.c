/*
lcd.c: functions to control the LCD
*/

#include <c8051f000.h>
#include <rtx51tny.h>
#include "lcd.h"

/*
global constants
*/

#define LCD_DATA    P0                      // definitions for LCD

sbit RS=P1^2;
sbit EN=P1^3;

void LCD_Command(char phrase){

    RS=0;                                   // write instruction
    LCD_DATA=phrase;
    EN=1;                                   // pulse enable signal
    os_wait(K_TMO, 1, 0);
    EN=0;
    os_wait(K_TMO, 1, 0);                   // wait for instruction 
                                            // to complete

}


void LCD_Write(char phrase){

    RS=1;
    LCD_DATA=phrase;
    EN=1;
    os_wait(K_TMO, 1, 0); 
    EN=0;
    os_wait(K_TMO, 1, 0);

}


void LCD_Init(void){

    LCD_Command(0x0F);                      // activate display 
                                            // activate cursor and blink
    LCD_Command(0x01);                      // clear display area
    LCD_Command(0x38);                      // DL=1 8-bit data length 
                                            // N=1 2-line display mode
                                            // F=0 5x8 dots display mode
}


void LCD_Clear(void){

    LCD_Command(0x01);

}


void LCD_Move(int row, int col){

    LCD_Command(0x80+(row<<6)+col);		// if row specified, shifts 1 bit 
										// to AC6 as 2-nd line starts at AC 40
}


char convertKey(int phrase){

    switch(phrase){
        
        // keypad conversion
        case 0:
            return 0x30;
        case 1:
            return 0x31;
        case 2:
            return 0x32;
        case 3:
            return 0x33;
        case 4:
            return 0x34;
        case 5:
            return 0x35;
        case 6:
            return 0x36;
        case 7:
            return 0x37;
        case 8:
            return 0x38;
        case 9:
            return 0x39;
        case 10:
            return 0x2A;
        case 11:
            return 0x23;
    }

    return 0xFF;

}


void writeTemp(float temp){

    int quotient;

    quotient=temp/10;
    LCD_Write(convertKey(quotient));
    temp=temp-quotient*10;
    quotient=temp;
    LCD_Write(convertKey(quotient));
    temp=10*(temp-quotient);
    LCD_Write(0x2E);                        // decimal point
    quotient=temp;
    LCD_Write(convertKey(quotient));

}


void writeCelsius(void){

    LCD_Write(0x20);                        // space
    LCD_Write(0xDF);                        // degree symbol
    LCD_Write(0x43);                        // 'C'

}


void writeMinFail(void){

    LCD_Write(0x54);                        // 'T'
    LCD_Write(0x65);                        // 'e'
    LCD_Write(0x6D);                        // 'm'
    LCD_Write(0x70);                        // 'p'
    LCD_Write(0x3C);                        // '<'
    LCD_Write(0x4D);                        // 'M'
    LCD_Write(0x69);                        // 'i'
    LCD_Write(0x6E);                        // 'n'

}


void writeMaxFail(void){

    LCD_Write(0x54);                        // 'T'
    LCD_Write(0x65);                        // 'e'
    LCD_Write(0x6D);                        // 'm'
    LCD_Write(0x70);                        // 'p'
    LCD_Write(0x3E);                        // '>'
    LCD_Write(0x4D);                        // 'M'
    LCD_Write(0x61);                        // 'a'
    LCD_Write(0x78);                        // 'x'

}


void writeNoFail(void){

    LCD_Write(0x54);                        // 'T'
    LCD_Write(0x65);                        // 'e'
    LCD_Write(0x6D);                        // 'm'
    LCD_Write(0x70);                        // 'p'
    LCD_Write(0x20);                        // ' '
    LCD_Write(0x4F);                        // 'O'
    LCD_Write(0x4B);                        // 'K'
	LCD_Write(0x20);                        // ' '

}













































































/*
lcd.h: LCD function definitions
*/

#ifndef _LCD_H
#define _LCD_H

/*
function declarations
*/

void LCD_Command(char phrase);
void LCD_Write(char phrase);
void LCD_Init(void);
void LCD_Clear(void);
void LCD_Move(int row, int col);

char convertKey(int phrase);
void writeTemp(float temp);
void writeCelsius(void);
void writeMinFail(void);
void writeMaxFail(void);
void writeNoFail(void);

#endif





















































/*
temperature.c: temperature monitor system implemented using RTX51-Tiny RTOS
for the SiLabs C8051F005DK
*/

#include <c8051f000.h>                  // SFR declarations
#include <rtx51tny.h>                   // RTX51-Tiny functions
#include "lcd.h"
#include "keypad.h"

#define INIT    0
#define ADC     1
#define LCD     2
#define KEYPAD  3

/*
16-bit SFR definitions
*/

sfr16 ADC0 = 0xBE;                      // ADC0 data
sfr16 TMR3RL = 0x92;                    // Timer 3 reload value
sfr16 TMR3 = 0x94;                      // Timer 3 counter

/*
global constants
*/

#define INT_DEC 4096

sbit GLED = P1^0;
sbit RLED = P1^1;

/*
global variables
*/

float result;

int keyInput;

int minTemp, maxTemp, minmax;

int quotient, update, counter;

/*
task 0 (init): initialize
*/

void init(void) _task_ INIT {

    EA=0;                               // disable interrupts
    WDTCN=0xDE;                         // disable watchdog timer
    WDTCN=0xAD;

    OSCXCN=0x00;                        // external oscillator off
    OSCICN=0x07;                        // internal oscillator set 16 MHz

    XBR2=0x40;                          // enable cross-bar weak pull-up
    
    PRT0CF|=0xFF;                       // enable LCD data port
    PRT1CF|=0x0F;                       // enable RS/EN and LEDs
    PRT1CF|=0xF0;                       // enable keypad ports
    PRT3CF|=0xFF;                       // ditto

    ADC0CN=0x01;                        // disable ADC0, conversion on write
                                        // of 1 to ADBUSY, data is
                                        // left justified

    REF0CN=0x07;                        // enable temperature sensor, on-chip
                                        // VREF, VREF output buffer
    
    AMX0CF=0x08;
    AMX0SL=0x0F;                        // select int. temp. sensor 
    ADC0CF=0x80;                        // ADC0 conv. clock=CLK/16
    ADC0CF&=~0x07;                      // set PGA gain=1
    ADC0CN|=0x80;                       // enable ADC0

    TMR3CN=0x02;                        // stop timer 3, clear TF3 flag
                                        // use SYSCLK as timebase
    
    TMR3RL=-16000;                      // init. reload value
    TMR3=0xFFFF;                        // set to reload immediately
    EIE2|=0x01;                         // enable timer 3 interrupts
    TMR3CN|=0x04;                       // start timer 3
    
    EA=1;                               // enable interrupts

    LCD_Init();                         // initialize LCD

    minTemp=20;
    maxTemp=40;

    os_create_task(ADC);
    os_create_task(LCD);
    os_create_task(KEYPAD);
    os_delete_task(INIT);

}

/*
task 1 (adc0): ADC temperature measurements
*/

void adc0(void) _task_ ADC {

    long accumulator=0;
    int count=INT_DEC;

    while(1){

        ADBUSY=1;                       // start ADC conversion

        while(ADBUSY)
            ;                           // let conv. complete (ADBUSY -> 0)

        accumulator+=ADC0;
        count--;

        if(count==0){

            result=accumulator>>12;
            result=(result/131072*5-0.776)/0.00286;
            count=INT_DEC;
            accumulator=0;

        }

    }

}

/*
task 2 (LCD): update LCD
*/

void lcd(void) _task_ LCD {

    while (1){
        LCD_Clear();
        LCD_Move(0, 0);                        	// changed
        writeTemp(result);
        writeCelsius();
        LCD_Move(1, 0);                         // changed

        if (result<minTemp){
            writeMinFail();
            GLED=0;
            RLED=1;
        }

        else if(result>maxTemp){
            writeMaxFail();
            GLED=0;
            RLED=1;
        }

        else {
            writeNoFail();
            GLED=1;
            RLED=0;
        }

        os_wait(K_TMO, 255, 0);

    }

}

/*
task 3 (keypad): handle keypad input
*/

void keypad(void) _task_ KEYPAD {

    while (1){
        os_wait(K_SIG, 0, 0);
        if (keypadGetKey()>=10){
            os_delete_task(LCD);
            if (keypadGetKey()==10)
                minmax=0;
            else
                minmax=1;
            LCD_Clear();
            LCD_Move(0, 0);                     // changed
            LCD_Write(0x53);                    // 'S'
            LCD_Write(0x65);                    // 'e'
            LCD_Write(0x74);                    // 't'
            LCD_Write(0x20);                    // ' '
            LCD_Write(0x4D);                    // 'M'
            if (minmax==1){
                // write text for set max temp and clear var
                LCD_Write(0x61);                // 'a'
                LCD_Write(0x78);                // 'x'
                maxTemp=0;
            }
            else {
                // write text for set min temp and clear var
                LCD_Write(0x69);                // 'i'
                LCD_Write(0x6E);                // 'n'
                minTemp=0;
            }
			
			LCD_Move(1, 0);						// changed

            LCD_Write(0x54);                    // 'T'
            LCD_Write(0x3A);                    // ':'

            counter=0;
            
            while(counter<2){
                os_wait(K_SIG, 0, 0);
                if (minmax==1 && counter==0)
                    maxTemp+=keypadGetKey()*10;
                else if (minmax==0 && counter==0)
                    minTemp+=keypadGetKey()*10;
                else if (minmax==1 && counter==1)
                    maxTemp+=keypadGetKey();
                else if (minmax==0 && counter==1)
                    minTemp+=keypadGetKey();
                counter++;
                LCD_Write(convertKey(keypadGetKey()));
            }
        }
        os_create_task(LCD);
    }

}

void timer3ISR(void) interrupt 14 {
    if (keypadISR())
        os_send_signal(KEYPAD);
}

























































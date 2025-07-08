/*
keypad.c: Keypad functions
*/

#include <c8051f000.h>
#include "keypad.h"

sbit K0=P3^0;                       // definitions for keypad
sbit K1=P3^1;
sbit K2=P3^2;
sbit K3=P3^3;
sbit K4=P3^4;
sbit K5=P3^5;
sbit K6=P3^6;
sbit K7=P3^7;
sbit K8=P1^7;
sbit K9=P1^6;
sbit KS=P1^4;
sbit KP=P1^5;

int iCurrentKey;                    // hold input key
int iPressedKey;                    // holds detected key
int iDebounce=0;                    // debounce counter
int iStatus=0;

int keypadGetKey(void){

    return iCurrentKey;

}

bit keypadISR(void){

    iPressedKey=-1;
    
    // assuming only one key is pressed at a time, get the pressed key
    if (K0==0)
        iPressedKey=0;
    if (K1==0)
        iPressedKey=1;
    if (K2==0)
        iPressedKey=2;
    if (K3==0)
        iPressedKey=3;
    if (K4==0)
        iPressedKey=4;
    if (K5==0)
        iPressedKey=5;
    if (K6==0)
        iPressedKey=6;
    if (K7==0)
        iPressedKey=7;
    if (K8==0)
        iPressedKey=8;
    if (K9==0)
        iPressedKey=9;
    if (KS==0)
        iPressedKey=10;
    if (KP==0)
        iPressedKey=11;
    
    // if iStatus=0, then looking for valid key press
    // if iStatus=1, then looking for valid key release

    if (!iStatus && (iPressedKey!=-1)){

        if (iDebounce==0){
            iCurrentKey=iPressedKey;
            iDebounce++;
        } 

        else if (iDebounce<=5){

            if (iCurrentKey==iPressedKey)
                iDebounce++;
            else
                iDebounce=0;        // input still bouncing, reset

        }
        
        // assume only positive values for iDebounce. if key held for 6ms,
        // consider debounced, change status

        else {
            iStatus=1;
            iDebounce=0;
            return 1;
        }

    }

    else if (iStatus==1){
        
        if (iPressedKey==-1)        // no key pressed
            iDebounce++;
        else
            iDebounce=0;            // input still debouncing, reset
        
        if (iDebounce>5){

            // no key held for 6 ms
            iStatus=0;
            iDebounce=0;

        }

    }

    return 0;    

}






















































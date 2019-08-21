/*
 * 
 * *Program Title: Lab 1, Part 2 - L.E.D. Flash
 * 
 * Program File Name: lab1_part2.c
 * 
 * Miocroprocessors A 17.383
 * 
 * Spring 2019
 * 
 * Daniel Richter, Eric Swanson
 * 
 * 2/13/19
 */


#include "xc.h"

//PIC24HJ.... Configuration Bit Settings
//FBS
#pragma config BWRP = WRPROTECT_OFF  // Boot Segment Write Protect (Boot segment may be written)
#pragma config BSS = NO_FLASH        // Boot Segment Program Flash Code Protection (No Boot program flash segment)

// FGS
#pragma config GWRP = OFF           // General Code Segment Write Protect(User program memory is not write-protected)
#pragma config GSS = OFF            // General Segment Code Protection (User program memory is not code=protected)

//FOSCSEL
#pragma config FNOSC = FRC          // Oscillator Mode (Internal Fast RC(FRC))
#pragma config IESO = OFF           // Internal External Switch Over Mode (Start-up device with user-selected oscillator source)

//FOSC
#pragma config POSCMD = NONE        // Primary Oscillator Source (Primary Oscillator)
#pragma config OSCIOFNC = ON        // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF        // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSDCMD       // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS512      // Watchdog Timer Postscaler (1:512)
#pragma config WDTPRE = PR128       // WDT prescaler (1:128)
#pragma config WINDIS = OFF         // Watchdog Timer Window (Watchdog Timer in non-window mode)
#pragma FWDTEN = OFF                // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR16        // POR Timer value (16ms)
#pragma config ALTI2C = OFF         // Alternate I2C pins (I2C mapped to SDA1/SCL1 pins)
#pragma config ICS = PGD1           // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF         // JTAG Port Enable (JTAG is disabled)

void delay_routine(int x)
{
    unsigned short i, j; // 16 bits
    for(i = 0; i < x; i++)
        for (j = 0; j < x; j++);
        return;
}

/**************************************************/
/**
 * Function: main
 * 
 * Description: Flash RB15 on and off
 * 
 * Notes:
 * 
 * RB15 -
 * 
 * @return This routine contains an infinte loop
 */

int main(void) {
    /******* PIO Config *******/
    //Push Button
    _ODCB14 = 0;        // Disable open drain
    _TRISB14 = 0;       // Config RB14 as output
    _LATB14 = 0;        // RB14 initially low
    
    _ODCB13 = 0;        // Disables open drain
    _TRISB13 = 1; // Config RB13 as input
    _LATB13 = 1;        // RB13 initially low
    _CN13PUE =  1;      //Enable Weak-Pullup 

    _PCFG11 = 1;        //Disable A2D on RB13
    _PCFG10 = 1;	//Disable A2D on Rb14
    
    while(1 == 1) // loop forever
    {
        delay_routine(10); //used fofr debounce
        if(_RB13 == 0){
            
            _LATB14 = _LATB14 ^ 1; //toggle the state of RB14
        
            while (_RB13 == 0)
            {
                //keep RB13 lit until the button is pressed again
            }
        }
           
    }
        
    return 0;
}


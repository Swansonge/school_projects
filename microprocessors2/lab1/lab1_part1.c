/*
 * File:   lab1_part1.c
 * Author: Eric_Swanson
 *
 * Created on February 13, 2019, 7:47 PM
 */


#include "xc.h"
/*************
 *
 * Program title: Lab_1A-flash RB15
 * 
 * Program File Name: lab1_part1.c
 * 
 * Microprocessors 2
 * 
 * Spring 2019
 * 
 * Eric Swanson
 * 
 * 02/13/19 
 * 
 **************/

//PIC24HJ32GP202 Configuration Bit Settings
//FBS
#pragma config BWRP = WRPROTECT_OFF  //Boot segment write protect (boot segmetn may be written)
#pragma config BSS = NO_FLASH        //Boot segment Program Flash Code Protection(No boot program flash segment)

//FGS
#pragma config GWRP = OFF            //General Code Segment Write Protect (user program memory is not write-protected)
#pragma config GSS = OFF             //General segment code protection (user program memory is not code-protected)

//FOSCSEL
#pragma config FNOSC = FRC           //Oscillator Mode (internal fast RC (FRC))
#pragma config IESO = OFF            //Internal External Switch Over Mode (Start-up device user-selected oscillator source)

//FOSC
#pragma config POSCMD = NONE         //Primary Oscillator Source (primary oscillator disabled)
#pragma config OSCIOFNC = ON         //OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF         //Peripheral Pin Select Configuration (allow multiple re-configuration)
#pragma config FCKSM = CSDCMD        //Clock switching and monitor (both clock switching and fail-safe clock monitor are disabled)

//FWDT
#pragma config WDTPOST = PS512       //Watchdog Timer Postscaler (1:512)
#pragma config WDTPRE = PR128        //WDT Prescaler (1:128)
#pragma config WINDIS = OFF          //Watchdog Timer window (watchdog timer in nopn-window mode)
#pragma config FWDTEN = OFF          //Watchdog timer enable (watchdog timer enabled/disabled by user software)

//FPOR
#pragma config FPWRT = PWR16         //POR Timer Value (16ms)
#pragma config ALTI2C = OFF          //Alternate I2C pics (I2C mapped to SDA1/SCL1 pins)
//FICD
#pragma config ICS = PGD1            //Comm Channel select (communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF          //JTAG port enable (JTAG is disabled)

void delay_routine(void)
{
    unsigned short i,j;              //16 bits
    
            for (i=0; i<800; i++)
                for(j=0; j<800; j++);
                return;
}
/**********END OF delay_routine********/
/****************
 *Function: main
 * 
 * Description: FLash RB15 on and off
 * 
 * Notes
 * 
 * RB15 - 
 * 
 * Returns: this routine is an infinite loop
 * 
 ****************/
int main() {
/************PIO config*/
_ODCB15 = 1;                //Enable open drain
_TRISB15 = 0;               //Config RB15 as output
_LATB15=0;                  //RB15 initially low

while(1==1)                 //loop forever
{
    delay_routine();
    _LATB15                 //RB15 high ... LED on
    delay_routine();
    _LATB15 = 0;            //RB15 low ... LED off
} //***End of While (1==1) LOOP

//return
}
/*********END OF main ROUTINE*/


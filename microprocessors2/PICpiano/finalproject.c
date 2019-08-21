
/*
 * 
 * *Program Title: Course Project - PIC Piano
 * 
 * Program File Name: piano_code_easy.c
 * 
 * Microprocessors A 17.383
 * 
 * Spring 2019
 * 
 * Eric Swanson
 * 
 * 4/25/19
 * 
 * The purpose of this program is to create a one-octave piano using the PIC24HJ32GP202.
 * The "easy" version of this program uses a brute force method to determine which note should
 * be played through the speaker. Instead of using pulse width modulation functions on the PIC24
 * it uses sends a high signal to the speaker on the output and uses delays to keep the signal high
 * for a period of time determined by the note being played (button that is pressed). This method is
 * basically doing out PWM manually instead of using Timer2 and the Output compare module.
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>

#define    FCY    16000000UL    // Instruction cycle frequency, Hz - required for __delay_xx() to work
#include <libpic30.h>        // __delay_xx() function macros defined here

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

//These declarations assign the frequencies (in Hz) to their corresponding pitches/notes
//The variable names are in the solfege musical notation with do being the "root" (first) note
const unsigned int doh = 261; //do = C4 -> 261 Hz
const unsigned int di = 277;  //di = C#4 (Db4) -> 277 HZ
const unsigned int re = 293;  //re = D4 -> 293 Hz
const unsigned int ri = 311;  //ma = D#4 (Eb4) -> 311 Hz 
const unsigned int mi = 329;  //mi = E4 -> 329 Hz
const unsigned int fa  = 349; //fa = F4 -> 349 Hz
const unsigned int fi = 370;  //fi = F#4 (Gb4) -> 370 Hz
const unsigned int sol = 392; //sol = G4 -> 392 Hz
const unsigned int sih = 415; //si  = G#4 (Ab4) -> 415 Hz
const unsigned int la = 440;  //la = A4 -> 440 Hz
const unsigned int li = 466;  //li = A#4 (Bb4) -> 466 Hz
const unsigned int ti = 494;  //si = B4 -> 494 Hz
const unsigned int doh2 = 522;//do2 (second octave do) = C5 -> 522 Hz

void playSound(int note);

/********************************************************
* Function: playSound
*
* Description:   plays back a tone through the speaker depending on what button 
* is pressed (what note is desired). When each button is pressed, a square wave is sent
* to the speaker with the respective frequency. For example, when button1 is pressed
* C must be played. A square wave of 261 Hz is used to create this sound. But, this 
* frequency must be translated into a delay in microseconds (us) that controls the duty cycle.
* The wave will be high for the delay time (half the cycle) and then low for the delay time.
*
* 
* Notes: argument is the note that is desired (using the const int values defined)
* 
*
* Returns:  n/a
*
********************************************************/
void playSound(int note)
{
    //note is in Hz
    int period;             //Period for the entire wave. In us
    int cycle;              //Duty cycle delay (half high -> half low). In us
    period = 0;
    cycle = 0;
    
    period = (1/note)*1000000;
    cycle = period/2;
    
    //_LATB14 = _LATB14 ^ 1;  //Make the LED flash
    _LATA0 = 1;             //Set the speaker output to high
    __delay_us(cycle);      //Output is high for delay
    //_LATB14 = _LATB14 ^ 1;  
    _LATA0 = 0;             //Speaker output low
    __delay_us(cycle);      //Output low for delay
    return;
}


/********************************************************
* Function: main
*
* Description:  This configures the I/O pins and then completes the
* action of playing notes when the buttons are pressed.
*
* Notes:
* 
*
* Returns:  This routine contains an infinite loop
*
********************************************************/
void main(void){
    /****** IO CONFIG ******/
    //Configure all PORTA, PORTB as digital
    _PCFG0 = 1;        //Disable A2D on RA0
    _PCFG1 = 1;        //Disable A2D on RA1
    _PCFG4 = 1;        //Disable A2D on RB2
    _PCFG5 = 1;        //Disable A2D on RB3
    _PCFG9 = 1;        //Disable A2D on RB15
    _PCFG10 = 1;        //Disable A2D on RB14
    _PCFG11 = 1;        //Disable A2D on RB13
    _PCFG12 = 1;        //Disable A2D on RB12
    
    
    //Speaker
    _ODCA0 = 0;     //Disable open drain
    _TRISA0 = 0;    //Config RA0 as output
    _LATA0 = 0;     //RA0 initially low
    
    //C(lower octave) - do1
    _ODCA1 = 0;     //Disable open drain
    _TRISA1 = 1;    //Config RA1 as input
    _LATA1 = 1;     //Start low (push button off)
	_CN3PUE = 1;    //Enable Weak pull-up resistor on RA1
    __delay_us(1);  //delay to allow pull up on port to work
    
    //C# - di
    _ODCB2 = 0;     //Disable open drain
    _TRISB2 = 1;    //Config RB2 as input
    _LATB2 = 1;     //Start low (push button off)
    _CN6PUE = 1;    //Enable Weak pull-up resistor on RB2
    __delay_us(1);
    
    //D - re
    _ODCB3 = 0;     //Disable open drain
    _TRISB3 = 1;    //Config RB3 as input
    _LATB3 = 1;     //Start low (push button off)
    _CN7PUE = 1;    //Enable Weak pull-up resistor on RB3
    __delay_us(1);
    
    //D# - ri
    _ODCA2 = 0;     //Disable open drain
    _TRISA2 = 1;    //Config RA2 as input
    _LATA2 = 1;     //Start low (push button off)
    _CN30PUE = 1;   //Enable Weak pull-up resistor on RA2
    __delay_us(1);
    
    //E - mi
    _ODCA3 = 0;     //Disable open drain
    _TRISA3 = 1;    //Config RA3 as input
    _LATA3 = 1;     //Start low (push button off)
    _CN29PUE = 1;   //Enable Weak pull-up resistor on RA3
    __delay_us(1);
    
    //F - fa
    _ODCB4 = 0;     //Disable open drain
    _TRISB4 = 1;    //Config RB4 as input
    _LATB4= 1;      //Start low (push button off)
    _CN1PUE = 1;    //Enable Weak pull-up resistor on RB4
    __delay_us(1);
    
    //F# - fi
    _ODCA4 = 0;     //Disable open drain
    _TRISA4 = 1;    //Config RA4 as input
    _LATA4= 1;      //Start low (push button off)
    _CN0PUE = 1;    //Enable Weak pull-up resistor on RA4
    __delay_us(1);
    
    //G - sol
    _ODCB5 = 0;     //Disable open drain
    _TRISB5 = 1;    //Config RB5 as input
    _LATB5= 1;      //Start low (push button off)
    _CN27PUE = 1;   //Enable Weak pull-up resistor on RB5
    __delay_us(1);
    
    //G# - si
    _ODCB6 = 0;     //Disable open drain
    _TRISB6 = 1;    //Config RB6 as input
    _LATB6= 1;      //Start low (push button off)
    _CN24PUE = 1;   //Enable Weak pull-up resistor on RB6
    __delay_us(1);
    
    //A - la
    _ODCB7 = 0;     //Disable open drain
    _TRISB7 = 1;    //Config RB7 as input
    _LATB7= 0;      //Start low (push button off)
    _CN23PUE = 1;   //Enable Weak pull-up resistor on RB7
    __delay_us(1);
    
    //A# - li
    _ODCB8 = 0;     //Disable open drain
    _TRISB8 = 1;    //Config RB8 as input
    _LATB8= 1;      //Start low (push button off)
    _CN22PUE = 1;   //Enable Weak pull-up resistor on RB8
    __delay_us(1);
    
    //B - ti
    _ODCB9 = 0;     //Disable open drain
    _TRISB9 = 1;    //Config RB9 as input
    _LATB9= 1;      //Start low (push button off)
    _CN21PUE = 1;   //Enable Weak pull-up resistor on RB9
    __delay_us(1);
    
    //C(upper octave) - do2
    _ODCB10 = 0;     //Disable open drain
    _TRISB10 = 1;    //Config RB10 as input
    _LATB10= 1;      //Start low (push button off)
    _CN16PUE = 1;   //Enable Weak pull-up resistor on RB10
    __delay_us(1);
    
    //LED indicator
    _ODCB14 = 0;                //disable open drain
    _TRISB14 = 0;               //Config RB14 as output
    _LATB14=0;                  //RB14 initially low
    
    
    while(1){  
        //while the button is pressed, play the sound
        /*For each button some calculations are done to determine the delay needed for the 
         * duty cycle of the output to play the correct note:
         * Frequency of tone = F Hz
         * period = (1/F)*1000000 usec
         * delay for duty cycle to stay on/off is period/2
         * Example: C = 261 Hz
         * period = (1/261)*1000000 = 3831 usec
         * 3831/2 = 1915 -> output is high for 1915 usec and low for 1915 usec
         */
        
        while(_RA1 == 0){       //C
            
            
            _LATB14 = _LATB14 ^ 1;  //Make the LED flash
            _LATA0 = 1;             //Set the speaker output to high
            __delay_us(1915);      //Output is high for delay
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             //Speaker output low
            __delay_us(1915);      //Output low for delay
            _LATA1 = 1;
            //playSound(261);)
            
        }

        while(_RB2 == 0)         //C#
        {
            _LATB2 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1805);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1805);      
        }

        while(_RB3 == 0)         //D
        {
            _LATB3 = 1;
            _LATB2 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1706);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1706);      
        }

        while(_RA2 == 0)         //D#
        {
            _LATA2 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1607);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1607);      
            
        }

        while(_RA3 == 0)         //E
        {
            _RA3 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1519);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1519);      
        }

        while(_RB4 == 0)         //F
        {
            _RB4 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1433);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1433); 
        }

        while(_RA4 == 0)         //F#
        {
            _RA4 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1347);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1347); 
        }

        while(_RB5 == 0)         //G
        {
            _RB5 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1276);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1276); 
        }

        while(_RB6 == 0)         //G#
        {
            _RB6 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1205);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1205); 
        }

        while(_RB7 == 0)         //A
        {
            _RB7 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1136);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1136); 
        }

        while(_RB8 == 0)         //A#
        {
            _RB8 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1073);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1073); 
        }

        while(_RB9 == 0)         //B
        {
            _RB9 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(1012);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(1012); 
        }

        while(_RB10 == 0)         //C(high)
        {
            _RB10 = 1;
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 1;             
            __delay_us(958);      
            _LATB14 = _LATB14 ^ 1;  
            _LATA0 = 0;             
            __delay_us(958); 
        }
    }
}
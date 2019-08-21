
/*
 * 
 * *Program Title: Lab 2, Part 3 - DS1307 RTC Interfacing Program with the PIC24
 * 
 * Program File Name: lab4_part2.c
 * 
 * Microprocessors A 17.383
 * 
 * Spring 2019
 * 
 * Eric Swanson, Danny Richter
 * 
 * 4/10/19
 */


#include "xc.h"
#include <stdio.h>
#include <stdlib.h>


/********************************************************
*
* MPLAB X IDE v1.95 and XC16 v1.20
*
* * Pinout using the PIC24HJ32GP202
*
*	RB6     - LCD I/O D4
*       RB7     - LCD I/O D5
*       RB10    - LCD I/O D6
*       RB11    - LCD I/O D7
*	RA0     - LCD E Clocking Pin
*	RB5     - LCD R/S Pin

********************************************************/

// PIC24HJ32GP202 Configuration Bit Settings

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Mode (Internal Fast RC (FRC))
#pragma config IESO = OFF               // Internal External Switch Over Mode (Start-up device with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS512          // Watchdog Timer Postscaler (1:512)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR16            // POR Timer Value (16ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

#define E  _LATA0             		//  Define the LCD Control Pins
#define RS _LATB5

#define SLV_ADD 0xD0                    //Defines the address of the Slave (a.k.a the RTC)
					//7 bit address on RTC is Binary Coded Decimal (1101000) and LSB 0 indicates RTC will write

const int Twentyms = 3000;
const int Fivems = 1000;
const int TwoHundredus = 40;

//LCD Control Functions
void Configure_LCD_pins(void);
void LCDWrite(int LCDData, int RSValue);
void Init_LCD(void);
void Clear_LCD(void);
void LCD_Display(char Display[16]);
void wait_For_Idle();

//I2C Control Functions
void start();
void stop();
void restart();
void ack();
void nack();
void init_I2C();
void IdleI2C();
void handshake(uint8_t slaveAddress, uint8_t reg);
void send(uint8_t slaveAddress, uint8_t regToWrite, uint8_t data);
uint8_t receive(uint8_t slaveAddress, uint8_t regToRead);

//Time functions
void getDate(uint8_t currentDate[]);
void getTime(uint8_t currentTime[]);
void setDate(uint8_t currentDate[]);
void setTime(uint8_t currentTime[]);
void inputTimeDate(uint8_t currentTime[], uint8_t currentDate[], uint8_t hours,
                   uint8_t minutes, uint8_t seconds, uint8_t month,
                   uint8_t day, uint8_t year);

//Conversion Functions
uint8_t toDecimal(uint8_t BCDValue);
uint8_t toBCD(uint8_t DECValue);
void convertTimeDate(uint8_t array[]);

/********************************************************
* Function: main
*
* Description:  Read clock data from DS1307 and display on LCD
*
* Notes:
* 
*
* Returns:  This routine contains an infinite loop
*
********************************************************/

int main()
{
    char Display[20];                 //Stores values to be displayed
    uint8_t currentTime[3];
    uint8_t currentDate[3];
    
    inputTimeDate(currentTime, currentDate, 18, 44, 42, 4, 24, 19);      //Input current Time Values (x,x, Hr, Min, Sec, Month, Day, Year)
                                                                        //Time is 24 hr format
									//Time is hard-coded to be 5:08:42 PM on April 15, 2019, the first time we got the lab to run successfully 
    Configure_LCD_pins();             //Configures LCD Pins
    Init_LCD();                       //initialize the LCD Display
    Clear_LCD();                      //clear the LCD screen
    
    init_I2C();                       //I2C initialization 
    
    setTime(currentTime);	      //set time and date on the RTC using the hard-coded values
    setDate(currentDate);
    
    while(1 == 1)              // Loop Forever
    {
        getTime(currentTime);		//get time and date from the RTC
        getDate(currentDate);
        
        convertTimeDate(currentTime);		//convert time and date to display on the LCD
        convertTimeDate(currentDate);
        
        Clear_LCD(); // Clears the LCD screen
        sprintf(Display,"Time: %02d:%02d:%02d", currentTime[2], currentTime[1], currentTime[0]);
        LCD_Display(Display);
        
        LCDWrite(0b11000000, 0);    //  Move Cursor to the Second Line

        sprintf(Display,"Date: %02d/%02d/%02d", currentDate[1], currentDate[0], currentDate[2]);
        LCD_Display(Display);
     } //  End while
    
    
//return;
}

/******** END OF main ROUTINE ***************************/

void Configure_LCD_pins(void)
{
    _PCFG0 = 1;   //disable analog function on pin 2

    _TRISA0 = 0;   //RA0 is an output
    _TRISB5 = 0;   //PORTB 5-7, 10-11 are outputs
    _TRISB6 = 0;
    _TRISB7 = 0;
    _TRISB10 = 0;
    _TRISB11 = 0;

    _ODCA0 = 0;   //disable open drain on all LCD pins
    _ODCB5 = 0;
    _ODCB6 = 0;
    _ODCB7 = 0;
    _ODCB10 = 0;
    _ODCB11 = 0;

    _LATA0 = 0;  //initialize all LCD pins to 0
    _LATB5 = 0;
    _LATB6 = 0;
    _LATB7 = 0;
    _LATB10 = 0;
    _LATB11 = 0;
}  //end Configure_LCD_pins()

void LCDWrite(int LCDData, int RSValue)
{
    int n, k;

//  Get High 4 Bits for Output
    _LATB6 = (LCDData >> 4)  & 0b0001;
    _LATB7 = (LCDData >> 5)  & 0b0001;
    _LATB10 = (LCDData >> 6) & 0b0001;
    _LATB11 = (LCDData >> 7) & 0b0001;

    RS = RSValue;
    E = 1;
    E = 0;              		//  Toggle the High 4 Bits Out

//  Get Low 4 Bits for Output
    _LATB6 = LCDData  & 0b0001;
    _LATB7 = (LCDData >> 1) & 0b0001;
    _LATB10 = (LCDData >> 2) & 0b0001;
    _LATB11 = (LCDData >> 3) & 0b0001;
    RS = RSValue;
    E = 1;
    E = 0;              		//  Toggle the Low 4 Bits Out

    if ((0 == (LCDData & 0xFC)) && (0 == RSValue))
        n = Fivems;
    else
        n = TwoHundredus;
        
    for (k = 0; k < n; k++);    		//  Delay for Character

}  //  End LCDWrite

void Init_LCD(void)
{
    int i, j;
//  Initialize LCD according to the Web Page
    j = 2*Twentyms;
    for (i = 0; i < j; i++);            //  Wait for LCD to Power Up

// Initialize LCD 8 bit mode
    _LATB6 = 1;
    _LATB7 = 1;
    _LATB10 = 0;
    _LATB11 = 0;

    E = 1;
    E = 0;              		//  Send Command
    j = 3*Fivems;
    for (i = 0; i < j; i++);

    E = 1;
    E = 0;              		//  Repeat Command
    j = 8*TwoHundredus;
    for (i = 0; i < j; i++);

    E = 1;
    E = 0;              		//  Repeat Command Third Time
    j = 8*TwoHundredus;
    for (i = 0; i < j; i++);

//  Initialize LCD 4 Bit Mode
    _LATB6 = 0;
    _LATB7 = 1;
    _LATB10 = 0;
    _LATB11 = 0;

    E = 1;
    E = 0;
    j = 4*TwoHundredus;
    for (i = 0; i < j; i++);

    LCDWrite(0b00101000, 0);    		//  LCD is 4 Bit I/F, 2 Line

    LCDWrite(0b00001000, 0);                    //  display off

    LCDWrite(0b00000001, 0);    		//  Clear LCD

    LCDWrite(0b00000110, 0);    		//  Move Cursor After Each Character

    LCDWrite(0b00001100, 0);    		//  Turn On LCD
}  						//  End Init_LCD

void Clear_LCD(void)
{
	LCDWrite(0b00000001, 0);    		//  Clear LCD

}  						//End Clear_LCD

void LCD_Display(char Display[16])
{
    int ind;
	for (ind = 0; Display[ind] != 0; ind++)
        LCDWrite(Display[ind], 1);
}  						// End LCD_Display

void delay_routine(void)
{

	unsigned short i, j;			// 16 bits
	
		for (i = 0; i < 800; i++)  
                for (j = 0; j < 800; j++);
	return;

}
/******** END OF delay_routine *************************/

void wait_For_Idle()
{
    while(I2C1CON & 0x1F);   //Acknowledge sequence not in progress
                            //Receive sequence not in progress
                            //Stop condition not in progress
                            //Repeated Start condition not in progress
                            //Start condition not in progress
    
    while(I2C1STATbits.TRSTAT); //Bit = 0 .... Master transmit is not in progress
}

void start()
{
    wait_For_Idle();
    I2C1CONbits.SEN = 1;     //Initiate Start Condition
    while(I2C1CONbits.SEN);  //Hardware clear at end of master Start sequence
                             //0 = Start condition not in progress
}

void stop()
{
    wait_For_Idle();
    I2C1CONbits.PEN = 1;    //Initiate Stop condition on SDAx and SCLx pins
                            //Hardware clear at end of master Stop sequence
    while(I2C1CONbits.PEN); //bit = 0 -- Stop condition not in progress
}

void restart()
{
    wait_For_Idle();
    I2C1CONbits.RSEN = 1;   //Initiate Repeated Start condition on SDAx and SCLx pins
                            //Hardware clear at the end of master repeated Start sequence
    
    while(I2C1CONbits.RSEN);//bit = 0 --- Repeated Start condition not in progress
}

void ack()
{
    wait_For_Idle();
    I2C1CONbits.ACKDT = 0;  //0 = Send ACK during Acknowledge
    I2C1CONbits.ACKEN = 1;  //1 = Initiate Acknowledge sequence on SDAx and SCLx pins and 
                            //transmit ACKDT data bit
                            //Hardware clear at end of master Acknowledge sequence
    while(I2C1CONbits.ACKEN);//bit = 0 -- Acknowledge sequence not in progress
}

void nack()                 //Acknowledge Data bit
{
    wait_For_Idle();        
    I2C1CONbits.ACKDT = 1;  //1 = Send NACK during Acknowledge
    I2C1CONbits.ACKEN = 1;  //1 = Initiate Acknowledge sequence on SDAx and SCLx pins and
                            //transmit ACKDT data bit
                            //Hardware clear at end of master Acknowledge sequence
    
    while(I2C1CONbits.ACKEN);//bit = 0 --- ACknowledge sequence not in progress
}

void init_I2C()
{
    I2C1BRG = 34;           //Baud Rate Generator
    I2C1CONbits.I2CEN = 1;  //0 = Disables the I2Cx module. All I2C pins are controlled by
                            //port functions
    I2C1CONbits.DISSLW = 1; //0 = Slew rate control disabled
}

void IdleI2C()
{
    while(I2C1STATbits.TRSTAT);//Wait for bus Idle
}

//Sets up preliminary conditions so the traget register in the PIC can receive data from the RTC
void commSetup(uint8_t slaveAddress, uint8_t reg){
    I2CTRN = slaveAddress;                                      //transmits Slave Address to notify slave
    while(I2CSTATbits.TRSTAT == 1 || I2CSTATbits.ACKSTAT == 1); //Wait for transmission to finish or salve to send ACK = 0)
    I2CTRN = reg;                                               //Transmit target register to slave
    while(I2CSTATbits.TRSTAT == 1 || I2CSTATbits.ACKSTAT == 1); //Wait for transmission to finish or salve to send ACK = 0)
}

/****************************************************************************
 *Description: Transmit Data to the slave from the master. Uses the start and
 * stop functions to begin and end the transfer, as well as the commSetup() 
 * function to setup communication with the slave
 * 
 * Arguments: Address of the slave, register of the slave to write to, and the 
 * the data to be written.
 * 
 * Returns: Nothing
 ***************************************************************************/

void send(uint8_t slaveAddress, uint8_t regToWrite, uint8_t data){
    start();                                                    //Start communication protocol
    commSetup(slaveAddress, regToWrite);                         //Setup communication with slave
    I2CTRN = data;                                              //place data in the transmit register and send it
    while(I2CSTATbits.TRSTAT == 1 || I2CSTATbits.ACKSTAT == 1); //Wait for transmission to finish or salve to send ACK = 0)
    stop();                                                     //Stop communication protocol
}

/****************************************************************************
 *Description: Receive data from the slave. The function is the same as send()
 * however the address must be transmitted to the slave first in order to establish
 * communication as well as the direction of data (Read vs. Write)
 * 
 * Arguments: Address of Slave Device, Register of slave device to read from
 * 
 * Returns: Nothing 
 ****************************************************************************/

uint8_t receive(uint8_t slaveAddress, uint8_t regToRead){
    uint8_t data;                                               //Initialize data variable
    
    start();                                                    //begin I2C protocol
    commSetup(slaveAddress, regToRead);                         //Setup communication with slave
    restart();                                                  //REstart communication in read mode
    I2CTRN = (slaveAddress | 0x0001);                           //Change slave address to read mode (LSM = 1)
    while(I2CSTATbits.TRSTAT == 1 || I2CSTATbits.ACKSTAT == 1); //Wait for transmission to finish or salve to send ACK = 0)
    I2C1CONbits.RCEN = 1;                                       
    while(I2CSTATbits.RBF == 0);                                //When RBF = 1, receive register is full
    I2C1CONbits.RCEN = 0;                                       
    nack();                                                     //Master sends NACK to indicate receive is over
    stop();                                                     //Stop I2C communication
    data = I2CRCV;                                              //Pull data from receive register and return it
    return data;
}

/*******************
 * Description: The following functions allow for setting and retrieving the 
 * current time on the RTC. These values are stored within two arrays, currentTime
 * and currentDate, which each contain three components (Hour,min, sec for example)
 * 
 * Arguments: Relevant time or date array
 * 
 * Returns: Nothing
 ********************/
//Get the date data from the RTC. Fill in the currentDate array with this data.
void getDate(uint8_t currentDate[]){
     uint8_t x;          // Loop counter
    
    // For each element in the array, receive from the slave device and the 
    // Corresponding register (date, month, year) - offset by 4 to account for starting in the date register in
   // the RTC clock
    for(x = 0; x < 3; x++){
        currentDate[x] = receive(SLV_ADD, x+4);
    }
}

//Get the time data from the RTC. Fill in the currentTime array with this data.
void getTime(uint8_t currentTime[]){
    uint8_t x;              // Loop counter
    
    // For each element in the array, receive from, the slave device and the 
    // Corresponding register (seconds, minutes, hours)
    for(x = 0; x < 3; x++){
        currentTime[x] = receive(SLV_ADD, x);
    }
}

//Set the date in the RTC registers using data from currentDate array
void setDate(uint8_t currentDate[]){
    uint8_t x;          // Loop counter
    
    // For each element in the array, transmit to the slave device and the 
    // Corresponding register - offset by 4 to account for starting register in
    // the RTC clock
    for(x = 0; x < 3; x++){
        send(SLV_ADD, x + 4, currentDate[x]);
    }
}

//Set the time in the RTC registers using data from currentTime array
void setTime(uint8_t currentTime[]){
    uint8_t x; 
    
    for(x = 0; x < 3; x++){
        send(SLV_ADD, x, currentTime[x]);
    }
}

//Fills in the current time and date in the currentTime[] and currentDate[] arrays
void inputTimeDate(uint8_t currentTime[], uint8_t currentDate[], uint8_t hours,
                   uint8_t minutes, uint8_t seconds, uint8_t month,
                   uint8_t day, uint8_t year){
    currentTime[2] = toBCD(hours) & 0b00111111; //Makes sure hours in 24 hour format
    currentTime[1] = toBCD(minutes);
    currentTime[0] = toBCD(seconds);
    
    currentDate[1] = toBCD(month);
    currentDate[0] = toBCD(day);
    currentDate[2] = toBCD(year);
    
}

/***********
 *Description: The following functions are meant to convert 8-bit Decimals to 
 *  8-bit BCD values and vice-a-versa. The convertTimeData converts the entire 
 * time and date arrays to BCD from their decimal input values. convertTimeData
 * converts the entire time array.
 * 
 * Arguments: Values to be converted
 * 
 * Returns: Converted Values
*************/
//Convert BCD values received from RTC into Decimal values that can displayed and understood on the LCD
uint8_t toDecimal(uint8_t BCDValue){
    
    uint8_t lower = BCDValue & 0b00001111;      //extract from the 1's place
    uint8_t upper = BCDValue >> 4;              //Extract from the 10's place
    uint8_t decValue = (upper * 10) + lower;    //Create Decimal value
    
    return decValue;
}

//convert decimal value into BCD value that is understood by the RTC
uint8_t toBCD(uint8_t DECValue){
     uint8_t bcdValue = 0b00000000;             // Base BCD value to modify
    uint8_t upper = DECValue / 10;              //Extract upper and lower Decimal values
    uint8_t lower = DECValue % 10;
    
    upper = upper << 4;                         //Move the position of the upper bits
    
    bcdValue |= upper;                          //Arrange values correctly and return
    bcdValue |= lower;
    
    return bcdValue; 
}

//convert a time or date from BCD to  decimal
void convertTimeDate(uint8_t array[]){
    uint8_t x;          // Loop counter
    
    // For each element of the array, convert from BCD to Decimal
    for(x = 0; x < 3; x++){
        array[x] = toDecimal(array[x]);
    }
}
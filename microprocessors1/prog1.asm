; *******************************************************************
;     Jacob Sword
;     Jack DeGregorio
;     Zachary Bindell
;     Pakin Pongcheewin
;     Program 1: Make LEDs go though predefined cycle
;     Display MSB on PIN 1
; *******************************************************************

#include <p16F1829.inc>
     __CONFIG _CONFIG1, (_FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF);
     __CONFIG _CONFIG2, (_WRT_OFF & _PLLEN_OFF & _STVREN_OFF & _LVP_OFF);

    errorlevel -302                     ;surpress the 'not in bank0' warning

    cblock 0x70                         ;shared memory location that is accessible from all banks
Delay1, Delay2                        
     endc

    ORG 0                               ;start of code
Start:
                                        
    banksel         OSCCON              
    movlw           b'00111000'         ;set cpu clock speed to 500KHz
    movwf           OSCCON              
                                       
					;Configure the LEDs
    banksel         TRISC              
    clrf            TRISC               ;make all of PORTC an output
    banksel         LATC 
    clrf	    LATC

MainLoop:
    call	    Table
    banksel	    LATC
    movwf	    LATC
    movlw	    d'180'
    movwf	    Delay2

DelayLoop:			           
    decfsz	    Delay1, F	    
    bra		    DelayLoop	    ;total delay = ((763 + 2) * 180) / 125k = approx 1s
    decfsz	    Delay2, f
    bra		    DelayLoop
    bra		    MainLoop

Table:
    banksel	    LATC	
    movf	    LATC, W
    addwf	    PCL, F	    ;table lookup to "reverse" bottom input bits
    retlw	    B'00001100'     ;so DS1 is MSB
    retlw	    B'00001001'
    nop
    nop
    nop
    retlw	    B'00001100'
    retlw	    B'00001010'
    nop
    retlw	    B'00001110'
    retlw	    B'00000110'
    retlw	    B'00000101'
    nop
    retlw	    B'00001000'
    nop
    retlw	    B'00000001'
    nop
    nop
    retlw	    B'00001100'    ;catch any error state and go back to first position
    
    end
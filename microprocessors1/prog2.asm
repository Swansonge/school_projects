; *******************************************************************
;     Jacob Sword
;     Jack DeGregorio
;     Zachary Bindell
;     Pakin Pongcheewin
;     Program 2: Increment count in binary for button presses.
;     Display MSB on PIN 1
; *******************************************************************

#include <p16F1829.inc>
     __CONFIG _CONFIG1, (_FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF);
     __CONFIG _CONFIG2, (_WRT_OFF & _PLLEN_OFF & _STVREN_OFF & _LVP_OFF);

#define     SWITCH  PORTA, 2            ;pin where SW1 is connected..NOTE: always READ from the PORT and WRITE to the LATCH

    errorlevel -302                     ;surpress the 'not in bank0' warning

    cblock 0x70                         ;shared memory location that is accessible from all banks
Delay1, prev                            ;delay loop for debounce
     endc

    ORG 0                              ;start of code
Start:
                                        
    banksel         OSCCON              
    movlw           b'00111000'         ;set cpu clock speed to 500KHz
    movwf           OSCCON              
    bsf             TRISA, RA2          ;switch as input
    banksel         ANSELA              ;bank3
    bcf             ANSELA, RA2         ;digital

                                        ;Configure the LEDs
    banksel         TRISC              
    clrf            TRISC               ;make all of PORTC an output
    banksel         LATC               
    clrf            LATC                ;start with all LEDs off

MainLoop:
    banksel         PORTA               
    btfsc           SWITCH              
    bra             NotPressed              
    bra             Debounce		;button is held, switch pin needs to be debounced            

Table:
    banksel	    LATC	
    movf	    LATC, W
    addwf	    PCL, F	     ;table lookup to "reverse" bottom input bits
    retlw	    B'00001000'	     ;because need MSB on pin DS1
    retlw	    B'00001001'
    retlw	    B'00001010'
    retlw	    B'00001011'
    retlw	    B'00001100'
    retlw	    B'00001101'
    retlw	    B'00001110'
    retlw	    B'00001111'
    retlw	    B'00000100'
    retlw	    B'00000101'
    retlw	    B'00000110'
    retlw	    B'00000111'
    retlw	    B'00000010'
    retlw	    B'00000011'
    retlw	    B'00000001'
    retlw	    B'00000000'
    
Increment:
    banksel         LATC                
    call	    Table
    movwf	    LATC
    movlw	    1
    movwf	    prev		;indicate successful buttondown
    bra		    MainLoop
    
Pressed:
    btfss	    prev, 0
    bra		    Increment
    movlw	    1
    movwf	    prev		;indicate successful buttondown
    bra		    MainLoop            ; go back to MainLoop
NotPressed:
    clrf	    prev		;no successful buttondown
    bra             MainLoop            

Debounce:
                                        ;delay for approximatly 5ms
    movlw           d'209'              ;(1/(500KHz/4))*209*3 = 5.016mS
    movwf           Delay1
DebounceLoop:
    decfsz          Delay1, f           ;1 instruction to decrement,unless if branching
    bra             DebounceLoop        ;2 instructions to branch

    banksel         PORTA               
    btfss           SWITCH              ;check if switch is still down.
    bra             Pressed		
    clrf	    prev
    bra             MainLoop
    
    end                                 
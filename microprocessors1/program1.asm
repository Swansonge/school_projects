; *******************************************************************
;   Eric Swanson  
;   Program 1: LEDs go through specified cycle
;   Display MSB on DS1
; *******************************************************************

#include <p16F1829.inc>
     __CONFIG _CONFIG1, (_FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF);
     __CONFIG _CONFIG2, (_WRT_OFF & _PLLEN_OFF & _STVREN_OFF & _LVP_OFF);

    errorlevel -302                     ;supress the 'not in bank0' warning
    cblock 0x70                         ;shared memory location that is accessible from all banks
Delay1                                  ;define two file registers for the delay loop in shared memory
Delay2
     endc


    ORG 0                               ;start of code
Start:
     banksel        OSCCON              ;bank1
     movlw          b'00111000'         ;set cpu clock speed of 500KHz
     movwf          OSCCON              ;move contents of the working register into OSCCON
     banksel	    TRISC
     clrf           TRISC               ;make all of PORTC an output
     banksel        LATC                ;select the bank where LATC is (bank2)
     clrf	    LATC		
     
MainLoop:
     call	    Table		;Lookup value to change LED to next
     banksel	    LATC		;Once That value is found, delay one sec
     movwf	    LATC
     movlw          d'162'		;Delay1 will equal 162 so the outer delay loops is 162 loops long
     movwf          Delay2
     
OndelayLoop:
     decfsz         Delay1,f            ;Waste time.
     goto           OndelayLoop         ;The Inner loop takes 3 instructions per loop * 256 loopss = 763 instructions
     decfsz         Delay2,f            ;The outer loop takes an additional 3 instructions per lap * 256 loops
     goto           OndelayLoop         ;(763+3) * 162 = 124092 instructions / 125K instructions per second = 0.993 sec.
     goto	    MainLoop

Table:
    banksel	    LATC
    movf	    LATC,W
    addwf	    PCL,F		;table lookup to select the right order for LEDS to light up
    retlw	    b'00001100'
    retlw	    b'00001001'
    nop		    
    nop
    nop					;next states don't exist for 2,3,4,7,11,13,15 so use nop
    retlw	    b'00001100'
    retlw	    b'00001010'
    nop
    retlw	    b'000001110'
    retlw	    b'00000110'
    retlw	    b'00000101'
    nop
    retlw	    b'00001000'
    nop
    retlw	    b'00000001'
    nop
    retlw	    b'00001100'		;catch errors and go back to the beginning
    
    end                                 ;end code section
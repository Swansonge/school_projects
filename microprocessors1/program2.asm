; *******************************************************************
; Eric Swanson
; Program 2: count number of times pushbutton is pressed in binary on LEDs
; *******************************************************************

#include <p16F1829.inc>
     __CONFIG _CONFIG1, (_FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF);
     __CONFIG _CONFIG2, (_WRT_OFF & _PLLEN_OFF & _STVREN_OFF & _LVP_OFF);

;#define     DOWN    0                  ;when SW1 is pressed, the voltage is pulled down through R3 to ground (GND)
;#define     UP      1                  ;when SW1 is not pressed, the voltage is pulled up through R1 to Power (Vdd)

#define     SWITCH  PORTA, 2            ;pin where SW1 is connected..NOTE: always READ from the PORT and WRITE to the LATCH

    errorlevel -302                     ;surpress the 'not in bank0' warning

    cblock 0x70                         ;shared memory location that is accessible from all banks
Delay1,indic                            ;delay loop for debounce
     endc


    ORG 0                              ;start of code
Start:
                                        ;Setup main init
    banksel         OSCCON              ;bank1
    movlw           b'00111000'         ;set cpu clock speed to 500KHz
    movwf           OSCCON              ;move contents of the working register into OSCCON
                                        ;Setup SW1 as digital input
    bsf             TRISA, RA2          ;switch as input
    banksel         ANSELA              ;bank3
    bcf             ANSELA, RA2         ;digital
                                        ;can reference pins by their position in the PORT (2) or name (RA2)

                                        ;Configure the LEDs
    banksel         TRISC               ;bank1
    clrf            TRISC               ;make all of PORTC an output
    banksel         LATC                ;bank2
    clrf            LATC                ;start with all LEDs off
    
MainLoop:
    banksel         PORTA               ;get into Bank0
    btfsc           SWITCH 
    bra             NotPressed             
    bra             Debounce            ;switch is held down, pin needs to be debounced

Table:
    banksel	    LATC	
    movf	    LATC, W		;using current value of the LED, select next state
    addwf	    PCL, F		;Pins to be viewed are reversed of actual pins in PIC, so cannot simply increment value and move to the output
    retlw	    b'00001000'
    retlw	    b'00001001'
    retlw	    b'00001010'
    retlw           b'00001011'
    retlw	    b'00001100'
    retlw	    b'00001101'
    retlw	    b'00001110'
    retlw	    b'00001111'
    retlw	    b'00000100'
    retlw	    b'00000101'
    retlw	    b'00000110'
    retlw	    b'00000111'
    retlw	    b'00000010'
    retlw	    b'00000011'
    retlw	    b'00000001'
    retlw	    b'00000000'

Increment:
    banksel         LATC                
    call	    Table
    movwf	    LATC
    movlw	    1
    movwf	    indic		;indicate successful buttondown
    bra		    MainLoop
    
Pressed:
    btfss	    indic, 0
    bra		    Increment
    movlw	    1
    movwf	    indic		;indicate successful buttondown
    bra		    MainLoop            ; go back to MainLoop  
NotPressed:
    clrf	    indic		;no successful buttondown
    bra             MainLoop            
    
Debounce:
                                        ;delay for approximatly 5ms
    movlw           d'209'              ;(1/(500KHz/4))*209*3 = 5.016mS
    movwf           Delay1
DebounceLoop:
    decfsz          Delay1, f           ;1 instruction to decrement,unless if branching (ie Delay1 = 0)
    bra             DebounceLoop        ;2 instructions to branch

    banksel         PORTA               ;bank0
    btfss           SWITCH              ;check if switch is still down. 
    bra		    Pressed		;if pressed, go to appropriate loop
    clrf	    indic		;if not, indicator variable
    bra             MainLoop
    
    end                                 ;end code
; *******************************************************************
; Eric Swanson
; Program 3 Rotate LEDs using the pot. Use button to interrupt and stop the program 
; *******************************************************************

#include <p16F1829.inc>
     __CONFIG _CONFIG1, (_FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF);
     __CONFIG _CONFIG2, (_WRT_OFF & _PLLEN_OFF & _STVREN_OFF & _LVP_OFF);

#define     DOWN    0x00                ;when SW1 is pressed, the voltage is pulled down through R3 to ground (GND)
#define     UP      0xFF                ;when SW1 is not pressed, the voltage is pulled up through R1 to Power (Vdd)
#define     SWITCH  PORTA, 2            ;pin where SW1 is connected..NOTE: always READ from the PORT and WRITE to the LATCH

#define     LED_RIGHT   1               ;keep track of LED direction
#define     LED_LEFT    0

    errorlevel -302                     ;surpress the 'not in bank0' warning

    cblock 0x70                         ;shared memory location that is accessible from all banks
Delay1                                  ;delay loop for debounce and LED delay
Delay2                                  ;delay loop for LED delay
Direction                               ;either 1 or 0 depending on what direction LEDs are going
Previous_Direction                      ;don't keep toggling the direction if the switch is held down
Pause					;tells when light is paused
LastButton				;For telling last button press
Temp					;Temp value used for CheckMid function to tell if pot is in the middle
     endc


     ORG 0                              ;start of code
Start:
                                        ;Setup main init
     banksel        OSCCON              ;bank1
     movlw          b'00111000'         ;set cpu clock speed of 500KHz
     movwf          OSCCON              ;move contents of the working register into OSCCON

                                        ;Configure the ADC/Potentimator
                                        ;already in bank1
     bsf            TRISA, 4            ;Potentimator is connected to RA4....set as input
     movlw          b'00001101'         ;select RA4 as source of ADC and enable the module
     movwf          ADCON0
     movlw          b'00010000'         ;left justified - Fosc/8 speed - vref is Vdd
     movwf          ADCON1
     banksel        ANSELA              ;bank3
     bsf            ANSELA, 4           ;analog for ADC

                                        ;Setup SW1 as digital input
     banksel        TRISA               ;bank1
     bsf            TRISA, RA2          ;switch as input
     banksel        ANSELA              ;bank3
     bcf            ANSELA, RA2         ;digital
                                        ;can reference pins by their position in the PORT (2) or name (RA2)

                                        ;Configure the LEDs
     banksel        TRISC               ;bank1
     clrf           TRISC               ;make all of PORTC an output
     banksel        LATC                ;bank2
     movlw          b'00001000'         ;start with  DS1 ON
     movwf          LATC

     movlw          LED_RIGHT           ;start with LEDs shifting to the right
     movwf          Direction

     ;Clear the RAM
     clrf           Previous_Direction
     clrf           Delay1
     clrf           Delay2
     clrf	    Pause
     clrf	    Temp

MainLoop:

     call           A2d                 ;get the ADC result
                                        ;top 8 MSbs are now in the working register (Wreg)
     movwf          Delay2              ;move ADC result into the outer delay loop
     movlw	    d'128'
     subwf	    Delay2, F		;delay2 - half of max value
     btfsc	    STATUS, Z
     bra	    Zero		;account for when ADC result delay2 - 128 == 0
     btfss	    STATUS, C
     bra	    Negative		;result is negative, delay2 - 128 leads to carry bit = 0
     movf	    Delay2, W		;result is positive, C == 1
     sublw	    d'128'		;128 - delay, number is between 0 and 128
     movwf	    Delay2
     call	    CheckMid		;check if around middle of pot rotation to stop LED
     movwf	    Temp
     btfsc	    Temp, 0
     bra	    Zero		;go to zero if CheckMid returned 1
     clrf	    Direction		;set direction to 0 (right)
     bra	    HandleDelay

Zero:
    call	    Debounce		;check if switch was pressed
    bra		    MainLoop		;LEDs do NOT rotate
    
Negative:
    movlw	    d'128'
    addwf	    Delay2, F		;delay is positive again
    call	    CheckMid		;see if delay is within middle threshold to stop LED rotation
    movwf	    Temp
    btfsc	    Temp,0
    bra		    Zero		;If CheckMid returns 1, go to Zero function
    clrf	    Direction
    incf	    Direction, F	;Set direction to 1 (left)
    bra		    HandleDelay		
    
    
CheckMid:
    movf	    Delay2, w
    movwf	    Temp		;delay is copied to temp variable
    movlw	    d'108'		;any value between 108 and 128 will stop LED
    subwf	    Temp, F		
    btfsc	    STATUS, C
    retlw	    d'1'		;1 shows pot is at middle
    retlw	    d'0'		;0 shows pot is not at middle
    
HandleDelay:
    call	    Slow		;delay will always be visible
    call	    DelayLoop		;delay next LED in rotation from turning on
    call	    Debounce		;check if switch was button was pressed
    btfss	    Pause, 0
    call	    Rotate		;rotate LEDs
    bra		    MainLoop
    


Debounce:
    banksel         PORTA               ;bank0
    btfsc           SWITCH              ;check if switch is down
    bra             SwitchNotDown
                                        ;delay for approximatly 5ms
    movlw           d'209'              ;(1/(500KHz/4))*209*3 = 5.016mS
    movwf           Delay1
DebounceLoop:
    decfsz          Delay1, f           ;1 instruction to decrement,unless if branching (ie Delay1 = 0)
    bra             DebounceLoop        ;2 instructions to branch

                                        ;no need to bank again...already in PORTA bank from above
    btfsc           SWITCH              ;check if switch is still down.
    bra             SwitchNotDown       ;failed the debounce test
ToggleDirection:                        ;switch is still down, toggle LED direction if previous direction != current direction
    btfsc	    LastButton,0	;this function checks for a pause when the button is pressed to see if it should pause or unpause
    return
    movlw	    d'1'
    movwf	    LastButton
    movf	    Pause, w
    xorlw	    d'1'
    movwf	    Pause
    return
    
SwitchNotDown:
    clrf	    LastButton		;set button as not being pressed
    return

Slow:
    movlw	    d'12'		;anything below 12 will be set to 12 so it doesn't rotate too fast
    subwf	    Delay2, w		;subtract w from ADC result 
    btfsc	    STATUS, C		;if ADC result was above 12 (not 0) return to mainloop
    return
    movlw	    d'12'		;otherwise, ADC was <12 so set to 12
    movwf	    Delay2
    return


A2d:
                                        ;Start the ADC
    nop                                 ;requried ADC delay of 8uS => (1/(Fosc/4)) = (1/(500KHz/4)) = 8uS
    banksel         ADCON0
    bsf             ADCON0, GO          ;start the ADC
    btfsc           ADCON0, GO          ;this bit will be cleared when the conversion is complete
    goto            $-1                 ;keep checking the above line until GO bit is clear
    movf            ADRESH, w           ;Get the top 8 MSbs

    return

DelayLoop:
                                        ;Delay amount is determined by the value of the ADC
     decfsz         Delay1,f            ;will always be decrementing 255 here
     goto           DelayLoop           ;The Inner loop takes 3 instructions per loop * 255 loops (required delay)
     decfsz         Delay2,f            ;The outer loop takes and additional 3 instructions per lap * X loops (X = top 8 MSbs from ADC conversion)
     goto           DelayLoop

     return

Rotate:
     banksel        LATC                ;change to Bank2
     movlw          LED_RIGHT           ;check what direction currently in
     subwf          Direction, w        ;be sure to save in wreg so as to not corrupt 'Direction'
     btfsc          STATUS, Z
     bra            RotateRight         ;rotating right
     bra            RotateLeft


RotateRight:
     lsrf           LATC, f             ;logical shift right
     btfsc          STATUS,C            ;did the bit rotate into the carry?
     bsf            LATC,3              ;yes, put it into bit 3.
     bra            MainLoop
RotateLeft:
     lslf           LATC, f             ;logical shift left
     btfsc          LATC, 4             ;did it rotate out of the LED display?
     bsf            LATC, 0             ;yes, put in bit 0
     bra            MainLoop

     end                                ;end code
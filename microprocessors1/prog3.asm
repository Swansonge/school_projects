; *******************************************************************
;     Jacob Sword
;     Jack DeGregorio
;     Zachary Bindell
;     Pakin Pongcheewin
;     Program 3: Rotate LEDs changing speed and direction based on poteniometer setting
; *******************************************************************

#include <p16F1829.inc>
     __CONFIG _CONFIG1, (_FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF);
     __CONFIG _CONFIG2, (_WRT_OFF & _PLLEN_OFF & _STVREN_OFF & _LVP_OFF);

#define     SWITCH  PORTA, 2            ;pin where SW1 is connected..NOTE: always READ from the PORT and WRITE to the LATCH

#define     LED_RIGHT   1               ;keep track of LED direction
#define     LED_LEFT    0

    errorlevel -302                     ;surpress the 'not in bank0' warning

    cblock 0x70                         ;shared memory location that is accessible from all banks
Delay1                                  ;delay loop for debounce and LED delay
Delay2                                  ;delay loop for LED delay
Direction                               ;either 1 or 0 depending on what direction LEDs are going
Previous_Direction                     
Paused
PrevButton
Temp
     endc

     ORG 0                              ;start of code
Start:
                                        ;Setup
     banksel        OSCCON              
     movlw          b'00111000'         ;set cpu clock speed of 500KHz
     movwf          OSCCON              ;move contents of the working register into OSCCON

                                        ;Configure the ADC/Potentimator
                                        ;already in bank1
     bsf            TRISA, 4            ;Potentimator is connected to ra4
     movlw          b'00001101'         ;select RA4 as source of ADC and enable 
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

     ;Clear varaibles
     clrf           Previous_Direction
     clrf           Delay1
     clrf           Delay2
     clrf	    Paused
     clrf	    Temp

MainLoop:

     call           A2d                 ;get the ADC result
                                        ;top 8 MSbs are now in the working register (Wreg)
	
     movwf          Delay2              ;move ADC result into the outer delay loop
     movlw	    d'128'		
     subwf	    Delay2, F		;delay2 - half of max value (128)
     btfsc	    STATUS, Z
     bra	    Zero		;128 - 128 returns 0
     btfss	    STATUS, C
     bra	    Negative		;small number - 128 returns negative
     
     movf	    Delay2, W		;else positive
     sublw	    d'128'		;128 - delay (make number between 0 and 128)
     
     movwf	    Delay2		
     call	    CheckMiddle		;check if around threshold at middle to stop rotation
     movwf	    Temp
     btfsc	    Temp, 0
     bra	    Zero		;goto Zero if returned 1 from CheckMiddle function
     clrf	    Direction		;set direction to 0 (right)
     bra	    HandleDelay		

Zero:
    ;DOESNT ROTATE
     call           Debounce            ;check if SW1 was pressed
     bra            MainLoop            ;do this forever
     
Negative:
    movlw	    d'128'
    addwf	    Delay2, F		;make positive again
    call	    CheckMiddle		;see if withint middle threshold to stop rotation
    movwf	    Temp
    btfsc	    Temp, 0
    bra		    Zero		;goto zero if 1 returned from function
    clrf	    Direction	        
    incf	    Direction, F	;set direction to 1 (left)
    bra		    HandleDelay
    
    
CheckMiddle:
    movf	    Delay2, W
    movwf	    Temp		;copy delay to temp
    movlw	    d'110'		;any values between 110 and 128 should stop LED rotation
    subwf	    Temp, F
    btfsc	    STATUS, C
    retlw	    d'1'		;return 1 to signify at middle
    retlw	    d'0'		;return 0 to signify not at middle
    
    
HandleDelay:
     call           SlowDown		;make so delay always visible
     call           DelayLoop           ;delay the next LED from turning ON
     call           Debounce            ;check if SW1 was pressed
     btfss	    Paused, 0
     call           Rotate              ;rotate the LEDs
     bra            MainLoop            


Debounce:
    banksel         PORTA               ;bank0
    btfsc           SWITCH              ;check if switch is down
    bra             NotPressed
                                        ;delay for approximatly 5ms
    movlw           d'209'              ;(1/(500KHz/4))*209*3 = 5.016mS
    movwf           Delay1

DebounceLoop:
    decfsz          Delay1, f           
    bra             DebounceLoop        
                                        ;no need to bank again...already in PORTA bank from above
    btfsc           SWITCH              ;check if switch is still down.
    bra		    NotPressed		;failed the debounce test

ToggleDirection:                        ;if switch is still down, and previous state was up.  Toggle pause variable
    btfsc	    PrevButton, 0
    return
    movlw	    d'1'
    movwf	    PrevButton
    movf	    Paused, W
    xorlw	    d'1'
    movwf	    Paused
    
    return
    
NotPressed:
    clrf	    PrevButton		;set button to not pressed every loop it is unpressed
    
    return
    

SlowDown:
     movlw          d'12'               ;anything below 12 will be set to 12 so rotation doesn't go too fast
     subwf          Delay2, w           ;subtract wreg with the ADC result and save in wreg
     btfsc          STATUS, C           ;if the ADC result is NOT '0', then return to MainLoop (adc was above 12)
     return                             ;return to MainLoop
     movlw          d'12'               ;otherwise, adc was less than 12 so set to 12
     movwf          Delay2              ;move it into the delay location in shared memory (RAM)
     
     return                             

A2d:
                                        ;Start the ADC
    nop                                 ;requried ADC delay of 8uS => (1/(Fosc/4)) = (1/(500KHz/4)) = 8uS
    banksel         ADCON0
    bsf             ADCON0, GO          ;start the ADC
    btfsc           ADCON0, GO          ;this bit will be cleared when the conversion is complete
    goto            $-1                 ;keep checking the above line until GO bit is clear
    movf            ADRESH, w           ;Get the top 8 MSbs (remember that the ADC result is LEFT justified!)

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

     end                                
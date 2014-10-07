
;Push button demo programme for AW60 demo board
;Button pressed: change LED pattern
;***************************************************************************************

PTAD		EQU	$0000			;Port A data register
PTADD		EQU	$0001			;Port A DDR
PTBD		EQU	$0002			;Port B data register
PTBDD		EQU	$0003			;Port B DDR
PTDD		EQU	$0006			;Port D data register
PTDDD		EQU	$0007			;Port D DDR
PTFD            EQU     $000A                   ;Port F data register
PTFDD           EQU     $000B                   ;Port F DDR

ICGC1           EQU     $0048                   ;Control for system clock
ICGC2           EQU     $0049                   ;Control for system clock


SOPT1		EQU	$1802			;Misc controls inc COP
PTAPE           EQU     $1840                   ;Pull ups for port A
PTDPE		EQU	$184C			;Pull ups for port D


;***************************************************************************************


VARIABLES	EQU	$0080			;Start address in RAM for variables
PROGRAMME	EQU	$8000			;start address in ROM for programme
STACK		EQU	$0100			;Start address for the top of the stack 
VECTORS		EQU	$FFFE			;Start address for reset vector


;***************************************************************************************
BIT2		EQU	%00000100		;Define constant BIT2 to avoid numbers
						;within the main programme.
PATTERN		EQU	%00001111		;Initial pattern for LEDs
;***************************************************************************************


		ORG	VARIABLES

COUNT0		RMB	1			;Variable for inner loop of delay
COUNT1		RMB	1			;Variable for outer loop of delay
COUNTER		RMB	1			;this byte contains the running count
PRESSED		RMB	1			;Indicates that button is already pressed



;***************************************************************************************
;There are some additional setting up lines in order to turn on the pull-up on Port D2.
;*************************************************************************************** 

		ORG	PROGRAMME		;Set the programme counter
START

		LDHX	#STACK			;Set the stack pointer
		TXS

		LDA     SOPT1			;Turn off the watchdog
		AND     #%01111111
		ORA     #%00000001
		STA	SOPT1


		LDA     #%01110100              ;Select external crystal
		STA     ICGC1
		

		JSR	SHORT_DELAY		;Start up delay for crystal


		LDA     #%11111111              ;Set all the pins of port F as outputs
		STA     PTFDD

;***************************************************************************************
;The extra setting up lines follow...
;***************************************************************************************

		LDA	#BIT2			;Load the binary pattern corresponding
						;to the push button on port D2.
		STA	PTDPE			;Store the pattern in the pull-up register

		LDA	#PATTERN                ;This is the initial pattern
		STA	PTFD                    ;sent to the LEDs

;***************************************************************************************
;This simple programme contains several lines of code to test the push button on PTD2.
;If the button is not pressed (PTD2 = 1) then do nothing. If the button is pressed, then
;toggle the leds. Note that the sense of the button is: pressed = logic level "0".
;***************************************************************************************
RELEASED
		CLR	PRESSED			;Initial value of variable PRESSED = 0
		JSR     SHORT_DELAY             ;Debounce delay
COUNT_LOOP                                      ;Entry point of parking loop
		LDA	PTDD                    ;Test Port D
		AND	#BIT2                   ;Mask Bit 2
		BEQ	TEST_PRESS              ;Branch if button is pressed

		TST	PRESSED                 ;Otherwise test variable PRESSED
		BEQ	COUNT_LOOP              ;If not already pressed, return to parking loop
	
		BRA	RELEASED                 ;If already pressed, go to RELEASED
TEST_PRESS
		TST	PRESSED                 ;If button pressed, test PRESSED
		BNE	COUNT_LOOP              ;If already pressed return to parking loop
		
		LDA	PTFD                    ;If not already pressed, toggle LEDs
		EOR	#$FF                    
		STA	PTFD
		LDA	#1                      ;and set PRESSED = 1
		STA	PRESSED
		BRA	COUNT_LOOP		;Finally return to parking loop


;***************************************************************************************
;Delay subroutine
;***************************************************************************************

SHORT_DELAY
		LDA	#$40			;Multiplier for delay
		STA	COUNT1			;Store the multiplier in COUNT1
		CLR	COUNT0			;Set the inner loop counter to zero
DEL1
		DEC	COUNT0			;Decrement the inner loop counter
		NOP				;This instruction does nothing
		BNE	DEL1			;Has the inner loop reached zero yet?
       		DEC	COUNT1			;If not, back to the label DEL1
		BNE	DEL1			;Otherwise decrement the outer loop
		RTS				;counter and then back to label DEL1


;**************************************************************************************		


		ORG	VECTORS
		FDB	START



;***************************************************************************************

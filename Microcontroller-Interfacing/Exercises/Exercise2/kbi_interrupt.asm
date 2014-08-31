;*************************************************************************
;*	kbi_interrupt.asm                                                    *
;*                                                                       *
;*	MC9S08AW60 Evaluation board keyboard interrupt example               *
;*	- Switch SW3 onboard connected to Port D pin 3, KBI pin6;            *
;*	- Switch SW4 onboard connected to Port D pin 2, KBI pin5             *
;*                                                                       *
;*	Function:                                                            *
;*	on reset all LEDs will light on. If SW3 or SW4 pressed,              *
;*	an interrupt is generated, which set LEDs 0:3 to light on.           *
;*	More interrupts are genereated if SW3 or SW4 are pressed.            *
;*************************************************************************

; Include derivative-specific definitions
		INCLUDE 'derivative.inc'

FLASH	EQU		$2000
RAM		EQU		$0070
WATCH	EQU		$1802

		ORG		RAM
LED_on	DS.B	1               ; Define a variable VAR_D with a size of 1 byte

;Start program after reset
		ORG		FLASH
START_UP
		LDA		#$00
		STA		WATCH			; Turn off the watchdog timer
		
;Init_GPIO init code 
		LDA     #$FF
		STA     PTFDD
		MOV     #$0F, LED_on    ; Initialize VAR_D, used to control the LEDs
		LDA     #$FF
		STA     PTDPE           ; Port D is enabled with pull-up
		RSP						; Reset stack pointer
                
;Enable interrupt for Keyboard input
		LDA     #$60
		STA     KBI1PE           ; KBI1PE: KBIPE7=1, enable KBI function for pins 5 and 6 only
		BSET    $02, KBI1SC      ; KBI1SC: KBACK=1, to clear KBI flag 
		BSET    $01, KBI1SC      ; KBI1SC: KBIE=1, enable KBI 
                               
		CLI                      ; Enable interrupt

MAINLOOP
		LDA     LED_on           ; Simple routine       
		BRA		MAINLOOP

;Interrupt service routine for a keyboard interrupt generated upon the press of a pushbutton
;with a falling edge (transition from high logic level "1" to low logic level "0")                
LED_SWITCH
		BSET    $02, KBI1SC     ; clear KBI flag 
		LDA     LED_on                              
		EOR     #$FF            ; Toggle bits in VAR_D
		STA     PTFD            ; Output to light LEDs (port F)
		STA     LED_on          ; Store the new value to VAR_D                                                          
		RTI                

;INT_VECTOR		
		ORG     $FFD2
		DC.W    LED_SWITCH

		ORG     $FFFE
		DC.W    START_UP

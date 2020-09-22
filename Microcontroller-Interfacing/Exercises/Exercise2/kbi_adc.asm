;*************************************************************************
;*	kbi_adc.asm                                                      *
;*                                                                       *
;*	MC9S08AW60 Evaluation board keyboard interrupt example           *
;*	- Switch SW3 onboard connected to Port D bit 3, KBI pin6;        *
;*	- Switch SW4 onboard connected to Port D bit 2, KBI pin5         *
;*                                                                       *
;*	Function:                                                        *
;*	On reset, all LEDs are off. When either SW3 or SW4 are pressed,  *
;*	then the ADC channel 8 is read and sent to the LEDs.             *
;*************************************************************************

		INCLUDE	'derivative.inc' ; Include derivative-specific definitions

FLASH	    EQU		$2000
RAM		    EQU		$0070
WATCH	    EQU		$1802

ConvComp	EQU		%10000000	;Mask for Conversion Complete flag

		ORG		RAM
LED_on		DS.B		1		; Define a variable VAR_D with a size of 1 byte

;Start program after reset

		ORG		FLASH
START_UP
		LDA		#$00
		STA		WATCH		; Turn off the watchdog timer
		
;Init_GPIO init code 
		LDA     	#$FF
		STA     	PTFDD
		MOV    		#$0F, LED_on	; Initialize VAR_D, used to control the LEDs
		LDA     	#$FF
		STA    		PTDPE           ; Port D is enabled with pull-up
		RSP				; Reset stack pointer to $0080
                
;Enable interrupt for Keyboard input
		LDA     	#$60
		STA     	KBI1PE           ; KBI1PE: enable KBI function for pins 5 and 6 only
		BSET    	$02, KBI1SC      ; KBI1SC: KBACK=1, to clear KBI flag 
		BSET    	$01, KBI1SC      ; KBI1SC: KBIE=1, enable KBI 
                               
		CLI                      	; Enable interrupt

MAINLOOP
		LDA     LED_on           ; Simple loop with "dummy" operation
		BRA		MAINLOOP
		
;Interrupt service routine for a keyboard interrupt generated upon the press of a pushbutton
;with a falling edge (transition from high logic level "1" to low logic level "0")                
LED_SWITCH
		BSET    	$02, KBI1SC     ; Clear KBI flag 
		LDA     	#8		; Select analogue input 8 (the blue potentiometer).
		STA     	ADC1SC1         ; ADC conversion will start after a number is written to ADC1SC1 register.
ADCLOOP                
		LDA     	ADC1SC1         ; 
		AND		    #ConvComp	; Check the COCO bit (conversion complete flag).
		BEQ     	ADCLOOP         ; if not complete, wait in the ADC loop.
		LDA     	ADC1RL          ; if complete, read the ADC outcome (digital value) from the register.
		STA     	PTFD            ; display over LED bar
		RTI                

;INT_VECTOR		
		ORG     	$FFD2
		DC.W    	LED_SWITCH
		
		ORG    		$FFFE
		DC.W    	START_UP






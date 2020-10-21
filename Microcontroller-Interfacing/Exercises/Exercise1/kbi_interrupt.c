/* kbi_interrupt.c */

#include <hidef.h>	 /* for EnableInterrupts macro */
#include "derivative.h"	/* include peripheral declarations */
#define VNkeyboard 22   /* Interrupt vector for Keyboard */

typedef unsigned char muint8;
typedef unsigned short muint16;
typedef unsigned long muint32;

typedef char mint8;
typedef short mint16;
typedef long mint32;

muint8 LED_onseq;

void main(void) 
{

	SOPT = 0x00;		/* disable COP */

	/* begin LED/switch test */

	PTDPE = 0xFF;		/* enable port D pullups for push button switch interrupt */

	/* Init_GPIO init code */
	PTFDD = 0xFF;		/* set port F as outputs for LED operation */
	LED_onseq = 0x0F;	/* initialize LED_onseq */

	//*********************************************************************************
	//* KBI1PE7 * KBI1PE6 * KBI1PE5 * KBI1PE4 * KBI1PE3 * KBI1PE2 * KBI1PE1 * KBI1PE0 *
	//*********************************************************************************	
	// KBI1PE register; each bit selects the corresponding keyboard interrupt pin.

	//*********************************************************************************
	//* KBEDG7  * KBEDG6  * KBEDG5  * KBEDG4  *   KBF   *  KBACK  *  KBIE   *  KBIMOD *
	//*********************************************************************************
	// KBI1SC register; top four bits 0 = falling edge 1 = rising edge of corresponding
	// pins KBEDG7 to 4. KBF keyboard interrupt flag, KBACK acknowledges interrupt flag.
	// KBIE turns on the keyboard interrupt system, KBIMOD 0 = edge detection.

	KBI1SC_KBIE = 0;	//Make sure interrupt is OFF
	KBI1PE = 0b01100000;	//Turn on interrupts for pins 5 and 6 only
	KBI1SC_KBIMOD = 0;	//Make sure we are on edge operation
	KBI1SC_KBACK = 1;	//Clear any possible pending interrupts
	KBI1SC_KBIE = 1;	//Turn on selected keyboard interrupts

	EnableInterrupts;	// enable interrupts globally ("big switch")

	for(;;) 
	{

	}	/* loop forever */
		/* make sure that you never leave main! */
}

// What follows is the interrupt service routine, which is called if either of the
// selected keyboard interrupts occurs on pins 5 and 6. However, Port D is tested
// and the LED toggle only happens if SW3 is pressed. (KBI 6, Port D3).

interrupt VNkeyboard void intKBI_SW()
{
	KBI1SC_KBACK = 1;		// acknowledge interrupt
	// this is the business of the interrupt
	if (PTDD_PTDD3 == 0)
	{
		PTFD = LED_onseq;	
		LED_onseq ^= 0xFF;	// toggle LED_onseq bits
	}	
	// do nothing if not keyboard interrupt VNkeyboard
}

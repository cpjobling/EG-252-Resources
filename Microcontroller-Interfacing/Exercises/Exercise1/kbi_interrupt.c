/* kbi_interrupt.c */

#include <hidef.h>		  /* for EnableInterrupts macro */
#include "derivative.h"	/* include peripheral declarations */
#define VNkeyboard 22   /* Interrupt vector for Keyboard */

typedef unsigned char muint8;
typedef unsigned short muint16;
typedef unsigned long muint32;

typedef char mint8;
typedef short mint16;
typedef long mint32;

/* to clear or set single bits in a byte variable */
#define b_SetBit(bit_ID, varID)		(varID |= (muint8)(1<<bit_ID))
#define b_ClearBit(bit_ID, varID)	(varID &= ~(muint8)(1<<bit_ID))
#define b_XorBit(bit_ID, varID)		(varID ^= (muint8)(1<<bit_ID))

muint8 LED_onseq;

void main(void) {
	SOPT = 0x00;		/* disable COP */

	/* begin LED/switch test */

	PTDPE = 0xFF;		/* enable port D pullups for push button switch interrupt */

	/* Init_GPIO init code */
	PTFDD = 0xFF;		/* set port F as outputs for LED operation */
	LED_onseq = 0x0F;	/* initialize LED_onseq */

	/* enable interrupt for keyboard input */
	b_ClearBit(1, KBI1SC);	/* KBI1SC: KBIE=0, disable KBI interrupt request */
	KBI1PE = 0x60;			/* KBI1PE: KBIPE7=1, enable KBI function for pins 5 and 6 only */
	b_ClearBit(0, KBI1SC);	/* KBI1SC: KBIMOD=0, select edge-only detection */
	
	/* in defaut only falling edge events to be detected */
	b_SetBit(2, KBI1SC);	/* KBI1SC: KBACK=1, to clear KBI flag */
	b_SetBit(1, KBI1SC);	/* KBI1SC: KBIE=1, enable KBI */
	
	EnableInterrupts;	/* enable interrupts */

	for(;;) {
		__RESET_WATCHDOG(); /* feeds the dog */
	}	/* loop forever */
		/* please make sure that you never leave main */
}

interrupt VNkeyboard void intKBI_SW(){
	KBI1SC_KBACK = 1;	/*acknowledge interrupt*/
	PTFD = LED_onseq;
	LED_onseq ^= 0xFF;	/* toggle LED_onseq bits */
}

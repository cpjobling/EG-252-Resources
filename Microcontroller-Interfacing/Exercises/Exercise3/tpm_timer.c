#include <hidef.h>		// for EnableInterrupts macro
#include "derivative.h"		// include peripheral declarations

#define VNtpm2ch2 11   // Interrupt vector for timer 2 channel 2 


byte tof_cnt, period;
byte student_num[6] = {1,2,3,4,5,6};
byte digit_cnt = 6;

void main(void)
{
    SOPT   = 0x00;		// disable COP (watchtimer)


    ICGC1 = 0x74; 		// select external quartz crystal

/*The AW60 has an internal oscillator which runs at approx. 8 MHz, resulting in a system clock of 4 MHz.
 If the line above is included in your programme, the AW60 uses an external 4 MHz quartz crystal oscillator
 instead, resulting in a system clock of 2 MHz. This is more accurate than the internal oscillator, so that
 any time intervals defined in your programme will be correct to a fraction of a percent.*/
 

    // Init_GPIO init code 
   	PTFDD = 0xFF;		// configure port F as outputs for LEDs

    // configure TPM module 1
   	TPM1SC = 0x4F;		// format: TOF(0) TOIE(1) CPWMS(0) CLKSB(0) CLKSA(1) PS2(1) PS1(1) PS0(1)
  				// Turn on the overflow interrupt and set the prescaler to 128.

	TPM1MOD = 0x8000;	// set the counter modulo registers to hex 8000 = 32,768 decimal.
				//You do not need to make separate writes to the low and high bytes of
				//a 16-bit register such as TPM1MOD. The file "derivative.h" includes
				//macros so that 16 bit variables are split and are written separately.*/

    tof_cnt = 0;  		// initialize the number of timer overflow to 0.

    EnableInterrupts;		// enable interrupts; from now on the interrupts are active
  
    for(;;) {
    }				// loop forever
}

interrupt VNtpm2ch2 void TPM1_overflow()
{
    TPM1SC_TOF = 0;		//Clear the overflow interrupt flag.
   
    if (tof_cnt >= digit_cnt) {
        tof_cnt = 0;		// reset tof_cnt if larger than digit_cnt
    }

    PTFD = student_num[tof_cnt];
    tof_cnt++;
}





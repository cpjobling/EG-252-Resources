#include <hidef.h>		// for EnableInterrupts macro
#include "derivative.h"	// include peripheral declarations

byte tof_cnt, period;
byte student_num[5] = {1,2,3,4,5};
byte digit_cnt = 5;

void main(void)
{
    EnableInterrupts;	// enable interrupts
    SOPT   = 0x00;		// disable COP (watchtimer)

    // Init_GPIO init code 
    PTFDD = 0xFF;		// configure port F as outputs for LEDs

    // configure TPM module 1
    TPM1SC = 0x4F;		// format: TOF(0) TOIE(1) CPWMS(0) CLKSB(0) CLKSA(1) PS2(1) PS1(1) PS0(1)
    TPM1MODH = 0xFF;	// set the counter modulo registers
    TPM1MODL = 0xFF;

    tof_cnt = 0;  // initialize the number of timer overflow to 0.
  
    for(;;) {
    }	// loop forever
}

interrupt 11 void TPM1_overflow()
{
    byte  varTOF;
   
    varTOF = TPM1SC_TOF;	// clear TOF; first read and then write 0 to the flag
    TPM1SC_TOF = 0;
   
    if (tof_cnt > digit_cnt) {
        tof_cnt = 0;		// reset tof_cnt if larger than digit_cnt
    }

    PTFD = student_num[tof_cnt];
    tof_cnt++;
}

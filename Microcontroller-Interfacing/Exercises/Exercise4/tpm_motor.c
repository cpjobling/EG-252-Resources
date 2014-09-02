/*

Modified test Programme for new AW60 teaching board thanks to contributions from
Dr Tim Davies.

This version gives direction control of the two motors but with interrupts to
give PWM speed control.

There are two interrupts: the timer overflow for TPM1 timer, which sets the
overflow frequency of 100 Hz with bus clock rate 4MHz, and TPM1 channel 1 for
turn off the motor. The motors are turned off individually to give individual
speed control if required.

The lower four rocker switches 1-4 on Port A determine the direction of the two
motors, e.g. 00001010 is reverse both motors, 00000101 is forward both motors,
00000000 is braked.

*/

#include <hidef.h>      // for EnableInterrupts macro
#include "derivative.h" // include peripheral declarations

byte DRIVE;
byte MODCNTH = 0x4E, MODCNTL = 0x20;            // values for the modulo registers (0x4E20)
byte INIT_CHNCNTH = 0x40, INIT_CHNCNTL = 0x00;  // set the IOC register to 0x3000 for output compare function

void main(void)
{
    EnableInterrupts;   // enable interrupts
    SOPT   = 0x00;      // disable COP (watchtimer)

    // Init_GPIO init code 
    PTADD = 0x00;   // set port A as inputs for the rocker switches.
    PTAPE = 0xFF;
    PTFDD = 0xFF;   // set port F as outputs for LEDs
    PTGDD = 0xFF;   // set port G as outputs for motor drive where motors are connected to.
    PTGPE = 0xFF;   // enable port G pullups for motor drive.
   
    // configure TPM module 1
    TPM1SC   = 0x49;  // format: TOF(0) TOIE(1) CPWMS(0) CLKSB(0) CLKSA(1) PS2(0) PS1(0) PS0(1)
    TPM1MODH = MODCNTH; TPM1MODL = MODCNTL; // set the counter modulo registers to 0x4E20.

    // configure TPM1 channel 1
    TPM1C1SC = 0x50;     // TPM1 Channel 1
    TPM1C1VH = INIT_CHNCNTH; TPM1C1VL = INIT_CHNCNTL;   // set the channel 1 registers to 0x4000
  
    for(;;) {
        DRIVE = PTAD & 0x0F;		// read the motor direction settings from the rocker switches 1-4
        PTFD = DRIVE;
    }   // loop forever
}

interrupt 11 void TPM1SC_overflow()
{   // interrupt vector: Vtpm1
    byte varClear;
    
    varClear = TPM1SC;  // 2 steps to clear timer interrupt flags
    varClear = varClear & 0x7F;
    TPM1SC = varClear;

    PTGD = DRIVE;       // turn on motors as configured by DRIVE (port A switches).
}

interrupt 6 void TPM1C1SC_int()
{   // interrupt vector: Vtpm1ch1
    byte varClear;

    varClear = TPM1C1SC;    // 2 steps to clear timer interrupt flags
    varClear = varClear & 0x7F;
    TPM1C1SC = varClear;    
 
    PTGD = PTGD | 0x0F;     // set free-wheel mode for both motors instead of turn off
}

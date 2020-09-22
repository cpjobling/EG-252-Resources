/*

Modified test Programme for new AW60 teaching board thanks to contributions from
Dr Tim Davies.

This version gives direction control of the two motors but with interrupts to
give PWM speed control.

There are two interrupts: the timer overflow for TPM1 timer, which sets the
overflow frequency of 100 Hz with bus clock rate 2MHz, and TPM1 channel 1 to
turn off the motor. The motors are turned off individually to give individual
speed control if required.

The lower four rocker switches 1-4 on Port A determine the direction of the two
motors, e.g. 00001010 is reverse both motors, 00000101 is forward both motors,
00000000 is braked.

*/

#include <hidef.h>      // for EnableInterrupts macro
#include "derivative.h" // include peripheral declarations

byte DRIVE;
word REPEAT = 0x4E20; //Define a 16 bit value to write directly to the modulus
word INITPW = 0x3000; //Similarly, lets define the pulse width as a 16 bit value.

void main(void)
{
    SOPT = 0x00; // disable COP (watchtimer)

    ICGC1 = 0x74; // Select external 4 MHz quartz crystal.

    // Init_GPIO init code
    PTADD = 0x00; // set port A as inputs for the rocker switches.
    PTAPE = 0xFF; // turn on the pullups for port A
    PTFDD = 0xFF; // set port F as outputs for LEDs
    PTGDD = 0xFF; // set port G as outputs for motor drive where motors are connected.

    // configure TPM module 1
    TPM1SC = 0x48;    // format: TOF(0) TOIE(1) CPWMS(0) CLKSB(0) CLKSA(1) PS2(0) PS1(0) PS0(0)
    TPM1MOD = REPEAT; //write the 16-bit value 0x4E20 (20,000 decimal) to the modulus register

    // configure TPM1 channel 1
    TPM1C1SC = 0x50;  // TPM1 Channel 1 interrupt enabled, output compare, no external output
    TPM1C1V = INITPW; //write 16-bit value 0x3000 (12,000 approx) to the channel 1 register

    EnableInterrupts; // enable interrupts

    for (;;)
    {
        DRIVE = PTAD & 0x0F; // read the motor direction settings from the rocker switches 1-4
        PTFD = DRIVE;
    } // loop forever
}

interrupt 11 void TPM1SC_overflow()
{ // interrupt vector: Vtpm1

    TPM1SC_TOF = 0; // clear the overflow interrupt flag

    PTGD = DRIVE; // turn on motors as configured by DRIVE (port A switches).
}

interrupt 6 void TPM1C1SC_int()
{ // interrupt vector: Vtpm1ch1

    TPM1C1SC = 0; // clear the channel 1 interrupt flag

    PTGD = PTGD | 0x0F; // set free-wheel mode for both motors instead of turn off
}

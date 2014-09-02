---
title:  'Exercise 4: Timer and Pulse Width Modulation (TPM) Module'
subtitle: 'EG-252 Group Design Exercise -- Microcontroller Laboratory'
author:
  - Dr K. S. (Joseph) Kim
  - Dr Chris P. Jobling
affiliation: Swansea University
tags: [eg-252, microcontrollers laboratory, timer, pwm, tpm]
date: September 2014
...

This exercise is designed for understanding of the TPM modules of MC9S08AW60 MCU
and practice of the control of TPM registers in C programming. For this exercise
you will be provided with two sample C programs, one for generating a delay by
the timer function and the other for pulse-width modulation (PWM) signals for
driving motor, respectively. Electronic versions of the programs can be
downloaded from the BlackBoard. You are to carry out the following tasks with
this exercise:

- Use the sample program "tpm_timer.c" to understand configurations of and
programming with the TPM module 1 (TPM1) registers for timer function. 
- Modify the sample program "tpm_timer.c" to generate a delay by configuring the
TPM module 2 (TPM2) registers. 
- Use the sample program "tpm_motor.c" to understand how to generate PWM signals 
to drive two DC motors on the demonstration board with speed and direction control. 
- Modify the sample program "tpm_motor.c" to set PWM duty cycle and implement independent motor
speed control for the two DC motors.

This exercise is worth 14 Marks. You should demonstrate your results over the
demonstration board to Dr Chris Jobling or Dr Tim Davies no later than
_Monday, 8 December 2014_. Dr Jobling will be available in the Lab to collect
your final reports from _11 to 12, Friday, 12 December 2014_.

You can view this document as a web page [HTML](exercise4.html), [PDF](exercise4.pdf) or as a Word Document [.docx](exercise4.docx)

## I. Task 1: Experiment with Sample Program "[tpm_timer.c](https://github.com/cpjobling/EG-252-Resources/tree/master/Microcontroller-Interfacing/Exercises/Exercise4/tpm_timer.c)"

This sample C program is designed to display 5 digits included a student
number 12345 over the LED bar. Each digit is displayed for a configured time,
which is determined by the timer modulo registers `TPM1MODH`:`TPM1MODL` of the
TPM1 module (lines 19 and 20). An interrupt is generated upon the timer
overflow, which is configured through the TPM1 status and control register
`TPM1SC` (line 18). More details on the registers and their configurations are
referred to the Freescale MC9S08AW60 datasheet document, Chapter 10 for
Timer/PWM.

Once the timer overflow interrupt is generated, the corresponding interrupt
service routine, i.e., `TPM1_overflow()` in this example, will be executed.
This interrupt service routine is associated to the timer overflow interrupt
by preceding the designated word “interrupt” and followed by the timer
overflow interrupt vector “11” in line 28. The interrupt vectors associated
with various interrupt sources can be obtained from the Freescale MC9S08AW60
datasheet. 

The lines 32 and 33 are a two-step procedure to clear the timer
overflow flag (TOF) in the `TPM1SC` register. Note that once the TOF is set, it
will not be automatically cleared. If the TOF is not cleared, the timer
overflow routine `TPM1_overflow` (line 28) will be continuously called. The
following method introduced in the MC9S08AW60 datasheet should be used to
clear the overflow flag: 

- Line 32: Read the TOF (`TPM1SC_TOF` as we are using the
TPM1 module). Note that you can address a register or a specific bit in a
register using the name of that register or that bit in a register. Their
names can be obtained from the MC9S08AW60 datasheet. For sample, if you want
to read the content of TPM1SC register, you can use "`varTOF = TPM1SC;` if you
want to read the content of THE TOF flag in the `TMP1SC` register, you can simply
use "`varTOF = TPM1SC_TOF`". The registers and flags that can be recognized by
CodeWarrior are highlighted by blue colour in the editor window. 

- Line 33: Write a zero to the overflow flag TOF by "`TPM1SC_TOF = 0`"

~~~~{include="tpm_timer.c" #tpm_timer_c .c .numberLines}
/* tpm-timer.c */
~~~~~~~~~~
View on [GitHub](https://github.com/cpjobling/EG-252-Resources/blob/master/Microcontroller-Interfacing/Exercises/Exercise4/tpm_timer.c)" 

Listing 1. Sample program for TPM timer function.


## II. Task 2: Modify the Sample Program to Display your Student Number 

For this task you are required to display all the digits in
your student number. 

- Each digit needs to be displayed for approximately one
second. You can control the modulo registers to generate the desired delay. 
- Instead of using the TPM1 module, you are required to use the TPM2 module to
generate the delay. Your job is to find correct TPM registers to use and
configure them accordingly. This task is worth of 5 marks. 

# III. Task 3: Experiment with Sample Program "[tpm_motor.c](https://github.com/cpjobling/EG-252-Resources/tree/master/Microcontroller-Interfacing/Exercises/Exercise4/tpm_motor.c) 

This sample C program is designed to generate PWM signals to drive two motors.
The motor speeds are controlled by the PWM signal duty cycle (i.e., ‘on’ time
with respect to a total period), while the motor directions (brake, forward
and reverse etc) can be controlled by the logic levels of output signals to
the motors.

With
this sample program the motor direction is configured by the setting of the
rocker switches. The motor speed is controlled by configuring the modulo
registers and the channel value registers. The modulo registers determine the
PWM signal period. Upon the timer overflow interrupt the motors are turned on
and direction is set by writing to port G where the two motors are connected.
Upon the output compare interrupt (when the free-running counter value matches
that stored in the channel value registers, an output compare interrupt is
generated if interrupt is enabled for that channel) both motors are turned
off. 

Note that to drive the motor, an additional power supply is needed, which
is available in the laboratory. Connect a 6-volt output to the demonstration
board. Compile and run the sample program, you will be able to try different
motor directions by setting the rocker switches accordingly. 

In this sample program there is a main program (Listing 2: Lines 27--51) and
two interrupt service routines (Listing 2: Lines 53--73). Use the MC9S08AW60
datasheet to find out the interrupt vectors for the TPM1 timer interrupt and
TPM1 channel 1 interrupt. Again check the MC9S08AW60 datasheet to understand
the configuration of the modulo register and IOC register.

~~~~{include="tpm_motor.c" #tpm_motor_c .c .numberLines}
/* tpm_motor.c */
~~~~~~~~~~

View on [GitHub](https://github.com/cpjobling/EG-252-Resources/blob/master/Microcontroller-Interfacing/Exercises/Exercise4/tpm_motor.c) 

## IV. Task 4: PWM Duty Cycle and Motor Speed Control

For Task 4 you are required to modify the sample program in order to: 

- *Set the duty cycle of the two motors to xx%*, where _xx_ is the leftmost
two digits of your student number. For example, if your student number is
567890, you should set the duty cycle to 56%. In the sample program, only TPM1
channel 1 is used to control the duty cycle (and motor speed) for both motors,
resulting identical speed for the two motors. *This part is worth of 4 marks*.

- *Enable independent speed control of the two motors*: Use the leftmost two
digits of two student numbers in your group to set duty cycle of two motors,
respectively. For example, if two student numbers are 123450 and 567890, you
should set the duty cycle of two motors to 12% and 56%, respectively. You are
expected to utilize another TPM1 channel (e.g. channel 0). Therefore two TPM1
channels are available for speed control, one channel per each motor. You
should introduce another interrupt service routine for the new TPM1 channel
and make changes to the interrupt service routine for TPM1 channel 1. _This
part is worth 5 marks_.

Listing 2 Interrupt service routines for timer overflow interrupt and
output compare interrupt. 

## Appendix 
### Theory of DC Motor Speed Control 

The
speed of a DC motor is directly proportional to the supply voltage, so if we
reduce the supply voltage from 12 volts to 6 volts, the motor will run at half
the speed. How can this be achieved when the battery is fixed at 12 volts? The
speed controller works by varying the average voltage sent to the motor. It
could do this by simply adjusting the voltage sent to the motor, but this is
quite inefficient to do. A better way is to switch the motor’s supply on and
off very quickly. If the switching is fast enough, the motor doesn’t notice
it, it only notices the average effect. 

When you watch a film in the cinema, or the
television, what you are actually seeing is a series of fixed pictures, which
change rapidly enough that your eyes just see the average effect - movement.
Your brain fills in the gaps to give an average effect. Now imagine a light
bulb with a switch. When you close the switch, the bulb goes on and is at full
brightness, say 100 watts. When you open the switch it goes off (0 watt). Now
if you close the switch for a fraction of a second, then open it for the same
amount of time, the filament won’t have time to cool down and heat up, and you
will just get an average glow of 50 watts. This is how lamp dimmers work, and
the same principle is used by speed controllers to drive a motor. When the
switch is closed, the motor sees 12 volts, and when it is open it sees 0 volt.
If the switch is open for the same amount of time as it is closed, the motor
will see an average of 6 volts, and will run more slowly accordingly. 

As the amount of time that the voltage is on increases compared with the
amount of time that it is off, the average speed of the motor increases. This
is the principle of switch mode speed control. Thus the speed is set by
PWM.[^1]

[^1]: Refer to [Speed Controllers](http://homepages.which.net/~paul.hills/SpeedControl/SpeedControllersBody.html) by Paul Hills for more details.

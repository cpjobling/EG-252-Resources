/*



Test Programme for the AW60 micromouse board


This version gives direction control of the two motors but with interrupts to
give PWM speed control. Inputs from the Hall Effect tachometers are used to
adjust the motor speeds so that the machine travels in a straight line.

Three interrupts are used to PWM the motors: the timer overflow for TPM1 timer,
which sets the overflow frequency of 100 Hz with bus clock rate 2MHz, turns both
motors on. TPM1 channel 1 and TPM1 channel 2 turn off the left and right motors.
Variables PWM_LEFT and PWM_RIGHT are the "instantaneous" values of PWM.

Two interrupts are used to measure the speed of the left and right motors.
Consecutive TPM2 channel 0 and channel 1 interrupts are subtracted to give
two variables DIFF_LEFT and DIFF_RIGHT which are used by the procedure "speedcon".


The main programme performs a simple avoidance action, using active low inputs from
the touch bars and active high inputs from the IR sensors.

Variable COUNTER is a 16-bit integer which is decremented every 10 ms.
Variable DISTANCE is a 16-bit integer which is decremented after every
tachometer pulse. One mm of travel is approximately 4 units in DISTANCE.



*/

#include <hidef.h>      // for EnableInterrupts macro
#include "derivative.h" // include peripheral declarations

#define TBFL PTAD_PTAD1

#define STOP 0b00000000
#define FWD 0b00000101
#define REV 0b00001010
#define ACW 0b00001001
#define CW 0b00000110
#define SFL 0b00000001 //Swerve forward and left
#define SFR 0b00000100 //Swerve forward and right

#define bitClear 0
#define SCALE 1

void revleft(void);
void revright(void);
void stop(void);
void reverse(void);
void turnleft(void);
void turnright(void);
void iravoidl(void);
void iravoidr(void);
void speedcon(void);

//The motors are connected to Port F bits 0 to 3 in pairs.
//Sense of switching is 01 = forward, 10 = reverse,
//00 = brake, 11 = freewheel.

byte DRIVE;
byte MODCNTH = 0x4E, MODCNTL = 0x20; // values for the modulo registers (0x4E20)

byte INIT_CHNCNTH = 0x40, INIT_CHNCNTL = 0x00; // set the IOC register to 0x3000 for output compare function

word REPEAT = 0x4E20;
word NOM_SPEED = 0x2000;
word PW_LEFT = 0x2000;
word PW_RIGHT = 0x2000;
word PW_MAX = 0x3000;
word PW_MIN = 0x1000;
word CORR = 0;

word COUNTER;
word DISTANCE;

word NEW_LEFT;
word OLD_LEFT;
word DIFF_LEFT;

word NEW_RIGHT;
word OLD_RIGHT;
word DIFF_RIGHT;

void main(void)
{

    SOPT = 0x00; // disable COP (watchtimer)

    ICGC1 = 0x74; // select external quartz crystal

    // Init_GPIO init code

    PTFDD = 0x0F; // set port F as outputs for motor drive on bits PTFD bits 0-3.

    PTDPE = 0b00001100; // use PTDD bits 2, 3 as input ports for touch bars

    // configure TPM module 1

    TPM1SC = 0x48; // format: TOF(0) TOIE(1) CPWMS(0) CLKSB(0) CLKSA(1) PS2(0) PS1(0) PS0(0)

    TPM1MOD = REPEAT; // set the counter modulo registers to 0x4E20 (= 20000 decimal).

    // configure TPM1 channel 1

    TPM1C1SC = 0x50; // TPM1 Channel 1

    TPM1C1V = NOM_SPEED; // set the channel 1 registers to 0x1000

    TPM1C2SC = 0x50; // TPM1 Channel 2

    TPM1C2V = NOM_SPEED; // set the channel 2 registers to 0x1000

    TPM2SC = 0x08;   //select bus clock for TPM2, no TOV
    TPM2C0SC = 0x44; //turn on edge interrupt for TPM2 C0
    TPM2C1SC = 0x44; //turn on edge interrupt for TPM2 C1

    //	PTBDD = 0b11111111;
    //	PTBD = 0;

    EnableInterrupts; // enable interrupts

    for (;;)
    {

        DRIVE = FWD;

        if ((PTDD & 0b00001100) != 0b00001100)
        {
            if ((PTDD & 0b00001000) == 0)
            {
                revleft();
            }
            if ((PTDD & 0b00000100) == 0)
            {
                revright();
            }
        }
        if ((PTDD & 0b11000000) != 0)
        {
            if ((PTDD_PTDD7 == 0) & (PTDD_PTDD6 == 1))
            {
                iravoidl();
            }
            if ((PTDD_PTDD7 == 1) & (PTDD_PTDD6 == 0))
            {
                iravoidr();
            }
            
             if ((PTDD_PTDD7 == 1) & (PTDD_PTDD6 == 1))
            {
                iravoidl();
            }
        }

    } // loop forever
}

interrupt 11 void TPM1SC_overflow()
{ // interrupt vector: Vtpm1

    TPM1SC_TOF = bitClear;

    PTFD = DRIVE; // turn on motors as configured by DRIVE (port A switches).

    speedcon();

    if (COUNTER != 0)
    {
        COUNTER--;
    }
}

interrupt 6 void TPM1C1SC_int()
{ // interrupt vector: Vtpm1ch1

    TPM1C1SC_CH1F = bitClear;

    PTFD = PTFD | 0x03; // set free-wheel mode for one motor instead of turn off
}

interrupt 7 void TPM1C2SC_int()
{ // interrupt vector: Vtpm1ch1

    TPM1C2SC_CH2F = bitClear;

    PTFD = PTFD | 0x0C; // set free-wheel mode for other motor instead of turn off
}

interrupt 12 void TPM2C0SC_int()
{
    TPM2C0SC_CH0F = bitClear;

    NEW_LEFT = TPM2C0V;
    DIFF_LEFT = NEW_LEFT - OLD_LEFT;
    OLD_LEFT = NEW_LEFT;
    if (DISTANCE != 0)
    {
        DISTANCE--;
    }

    //	PTBD = PTBD ^ 0B00000001;
}

interrupt 13 void TPM2C1SC_int()
{
    TPM2C1SC_CH1F = bitClear;

    NEW_RIGHT = TPM2C1V;
    DIFF_RIGHT = NEW_RIGHT - OLD_RIGHT;
    OLD_RIGHT = NEW_RIGHT;
    if (DISTANCE != 0)
    {
        DISTANCE--;
    }

    //	PTBD = PTBD ^ 0B00000010;
}

void revleft(void)
{
    stop();
    reverse();
    stop();
    turnleft();
    stop();
    //forward
}

void revright(void)
{
    stop();
    reverse();
    stop();
    turnright();
    stop();
    //forward
}

void stop(void)
{
    DRIVE = 0;
}

void reverse(void)
{
    DISTANCE = 200;
    DRIVE = REV;
    while (DISTANCE != 0)
    {
    }
}

void turnleft(void)
{
    DISTANCE = 100;
    DRIVE = ACW;
    while (DISTANCE != 0)
    {
    }
}

void turnright(void)
{
    DISTANCE = 100;
    DRIVE = CW;
    while (DISTANCE != 0)
    {
    }
}

void iravoidl(void)
{

    stop();
    turnleft();
    stop();
    //forward
}

void iravoidr(void)
{

    stop();
    turnright();
    stop();
    //forward
}

void speedcon(void)
{

    if (DIFF_RIGHT != DIFF_LEFT)
    {

        if (DIFF_RIGHT > DIFF_LEFT)
        {

            CORR = (DIFF_RIGHT - DIFF_LEFT) / SCALE;
            PW_LEFT = PW_LEFT + CORR;
            if (PW_LEFT > PW_MAX)
            {
                PW_LEFT = PW_MAX;
                PW_RIGHT = PW_MIN;
            }
            else
            {
                PW_RIGHT = PW_RIGHT - CORR;
            }
        }

        else
        {

            CORR = (DIFF_LEFT - DIFF_RIGHT) / SCALE;

            PW_RIGHT = PW_RIGHT + CORR;
            if (PW_RIGHT > PW_MAX)
            {
                PW_RIGHT = PW_MAX;
                PW_LEFT = PW_MIN;
            }
            else
            {
                PW_LEFT = PW_LEFT - CORR;
            }
        }
        //The next two lines may need to be swapped over
        TPM1C2V = PW_LEFT;
        TPM1C1V = PW_RIGHT;
    }
}
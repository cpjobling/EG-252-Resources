#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

// define new data types
typedef	unsigned char	muint8;
typedef enum {
  MOTOR_LEFT,
  MOTOR_RIGHT 
} Motor;
typedef enum {
  MOTOR_FORWARD,
  MOTOR_REVERSE,
  MOTOR_STOP
} MotorStatus;
    
// declare functions
void Forward(Motor);
void Reverse(Motor);
void Stop(Motor);
void Delay(int);
muint8 BitSet(muint8 Bit_position, muint8 Var_old);
muint8 BitClear(muint8 Bit_position, muint8 Var_old);

// constants for motor control
#define s1  0x4444
#define s2  0x2222
#define f   0xFFFF

// global variables for motor status
MotorStatus leftMotor, rightMotor;

void main(void)
{    
  EnableInterrupts;
  SOPT=0x00; //disable watchdog

  //--------------------------------------------------------
  // for motor control
  //--------------------------------------------------------
  TPM1SC=0b00001000; //setup timer1 for pwm on bus clk
  TPM1MOD=0x8888;     //set pwm period
  
  TPM1C2SC=0x28;     //setup ports f0-f3 for pwm
  TPM1C3SC=0x28;
  TPM1C4SC=0x28;
  TPM1C5SC=0x28;

  //--------------------------------------------------------
  // for KBI handling
  //--------------------------------------------------------
  PTDPE = 0xFF; // enable port D pullups for push button switch
  PTDDD = 0x00;	// set port D as input
    // Switch 3 and 4 are connected to port D3 and D2, respectively

  // KBI configuration
  KBI1SC = BitClear(1, KBI1SC); // KBIE=0 -> disable KBI
  KBI1PE = 0x60;	              // enable KBI function for pins 5 and 6 only
  KBI1SC = BitClear(0, KBI1SC);	// KBIMOD=0 -> select edge-only detection
  KBI1SC = BitSet(2, KBI1SC);   // KBACK=1 -> clear KBI flag 
  KBI1SC = BitSet(1, KBI1SC);   // KBIE=1 0> enable KBI 

  // initialise motor status
  leftMotor = MOTOR_STOP;
  rightMotor = MOTOR_STOP;
  
  for(;;) {
    // do nothing here; justwaiting for interrupts  
  }    // loop forever
}

//--------------------------------------------------------
// Functioins for motor driving
//--------------------------------------------------------
void Delay(int a){
     int b=0,c=0;
     for (b=0;b<a;b++){
          for(c=0;c<100;c++);
     }
}

void Forward(Motor motor){
  if (motor == MOTOR_LEFT) {
    TPM1C2V=s1;    //set pwm
    TPM1C3V=f;
  } else {
    TPM1C4V=s1;
    TPM1C5V=f;
  }
}

void Reverse(Motor motor){
  if (motor == MOTOR_LEFT) {
    TPM1C2V=f;    //set pwm     
    TPM1C3V=s1;
  } else {
    TPM1C4V=f;
    TPM1C5V=s1;
  }
}

void Stop(Motor motor){
  if (motor == MOTOR_LEFT) {
    TPM1C2V=f;    //set pwm     
    TPM1C3V=f;
  } else {
    TPM1C4V=f;
    TPM1C5V=f;
  }
}

//--------------------------------------------------------
// Functioins for KBI handling
//--------------------------------------------------------
// function to set a specific bit of an unsigned 8-bit interger
muint8 BitSet(muint8 Bit_position, muint8 Var_old)
{
        muint8 Var_new;
        Var_new = Var_old | (muint8)(1<<Bit_position);
        return Var_new;
}

// function to clear a specific bit of an unsigned 8-bit interger
muint8 BitClear(muint8 Bit_position, muint8 Var_old)
{
        muint8 Var_new;
        Var_new = Var_old & (~(muint8)(1<<Bit_position));
        return Var_new;        
}

//--------------------------------------------------------
// Interrupt service routines
//--------------------------------------------------------
interrupt 22 void intSW3_4()
{   // for KBI
// interrupt is a keyword. 
// 22 is the interrupt source number for keyboard interrupt module
    muint8 sw3, sw4;
    
  sw3 = PTDD & 0x08;
  sw4 = PTDD & 0x04;

  // simple FSM for motor status handling
  if (sw3 == 0) {
    // for left motor
    switch (leftMotor) {
      case MOTOR_STOP:
        Forward(MOTOR_LEFT);
        leftMotor = MOTOR_FORWARD;
        break;
      case MOTOR_FORWARD:
        Reverse(MOTOR_LEFT);
        leftMotor = MOTOR_REVERSE;
        break;
      case MOTOR_REVERSE:
        Stop(MOTOR_LEFT);
        leftMotor = MOTOR_STOP;
        break;
    }
  } else if (sw4 == 0) {
    // for right motor
    switch (rightMotor) {
      case MOTOR_STOP:
        Forward(MOTOR_RIGHT);
        rightMotor = MOTOR_FORWARD;
        break;
      case MOTOR_FORWARD:
        Reverse(MOTOR_RIGHT);
        rightMotor = MOTOR_REVERSE;
        break;
      case MOTOR_REVERSE:
        Stop(MOTOR_RIGHT);
        rightMotor = MOTOR_STOP;
        break;
    }
  }
/*
    if (sw3 == 0 & sw4 == 0) {
      Stop();
    } else if (sw3 == 0) {
      Reverse();
    } else if (sw4 == 0) {
      Forward();
    }
    // 'braking' hasn't been implemented yet
*/
   
  KBI1SC_KBACK = 1; // clear KBI interrupt flag
}

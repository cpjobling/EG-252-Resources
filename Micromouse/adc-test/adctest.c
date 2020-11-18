                                 /*



This is a programme fragment for testing the analogue inputs
and comparing with thresholds for black/white decisions.



*/

#include <hidef.h>      // for EnableInterrupts macro
#include "derivative.h" // include peripheral declarations

#define THRESH0 128	//Placeholder for black/white threshold
#define THRESH1 128	//Placeholder for black/white threshold
#define THRESH2 128	//Placeholder for black/white threshold
#define THRESH3 128	//Placeholder for black/white threshold

void adctest (void);
B
byte ADCMAP = 0;


void main(void)
{
	
  APCTL2 = 0b00001111;	//For example use bottom 4 bits of Port B  

	adctest();
	if (ADCMAP & 0b00000011 == 0b00000011){ /*something;*/}
	if (ADCMAP & 0b00000011 == 0b00000001){ /*something different;*/}

}
void adctest(void)
{
	ADCMAP = 0;

	ADC1SC1 = 0;	//kick off conversion on Port B bit 0
	while (ADC1SC1_COCO == 0){}
	if (ADC1RL>THRESH0){ADCMAP = ADCMAP + 1;}

	ADC1SC1 = 1;	//kick off conversion on Port B bit 1
	while (ADC1SC1_COCO == 0){}
	if (ADC1RL>THRESH1){ADCMAP = ADCMAP + 2;}

	ADC1SC1 = 2;	//kick off conversion on Port B bit 2
	while (ADC1SC1_COCO == 0){}
	if (ADC1RL>THRESH2){ADCMAP = ADCMAP + 4;}

	ADC1SC1 = 3;	//kick off conversion on Port B bit 3
	while (ADC1SC1_COCO == 0){}
	if (ADC1RL>THRESH3){ADCMAP = ADCMAP + 8;}
}



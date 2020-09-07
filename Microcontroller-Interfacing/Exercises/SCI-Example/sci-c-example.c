#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

typedef	unsigned char	muint8;
typedef	unsigned short	muint16;
typedef	unsigned long	muint32;

void Init_SCI(void);
void Send_string(muint8 * s);
void Ctoh (muint8 * s, muint8 c);
void Send_16b(muint16 i);
void Send_8b(muint8 c);
void Send_newline(void);
muint8 SCI_read(void);
void SCI_write_char(char data_out);


void main(void) {
  muint8 input_char;

  EnableInterrupts; /* enable interrupts */
  /* include your code here */
  SOPT   = 0x00;			 // Disable COP	
  
  PTFDD = 0xFF;
  PTFD = 0x55; 

  Init_SCI();
 
  Send_string("\r\n Hello World! \r\n");
  
  for (;;) {
     input_char = SCI_read(); 
     //Send_char(input_char);
     PTFD = input_char;
     SCI_write_char(input_char);
     Send_string(": ");
     Send_8b(input_char);
     Send_newline();
  } 
  /* please make sure that you never leave main */
}


//===================================================
void Init_SCI(void)
{
 
  ICGC1   = 0x78;   // select external crystal;
  ICGC2   = 0x30;   // multiply by 10; ICGC1 and ICGC2 specify 20 MHz bus clock
    
  SCI1BDH = 0x00;
  SCI1BDL = 0x82;   // Should give 9600 baud with 20 MHz bus clock
  SCI1C2  = 0x0C;   // Turn on transmitter and receiver.
  
}

//===================================================
/* send string via SCI */
//===================================================
void Send_string(muint8 * s)
{
  while (*s != 0)
  {
    while (!(SCI1S1 & 0x80));
    SCI1D=*s;
    s++;
  }
}


//===================================================
/* convert 8bit number to 2 hexadecimal ascii characters */
//===================================================
muint8 cvt[16]={'0','1','2','3','4','5','6','7','8','9',
                      'A','B','C','D','E','F' };

//===================================================
void Ctoh (muint8 * s, muint8 c)
{
  *s= cvt[c / 16];
  s++;
  *s= cvt[c % 16];
}


//===================================================
/* send 16bit number in hexa via SCI */
//===================================================
void Send_16b(muint16 i)
{
  muint8 s[6];
  s[0]=' ';
  Ctoh(s+1,(muint8)(i / 256));
  Ctoh(s+3,i % 256);
  s[5]=0;
  Send_string(s);
}


//===================================================
/* send 8bit number in hexa via SCI */
//===================================================
void Send_8b(muint8 c)
{
  muint8 s[4];
  s[0]=' ';
  Ctoh(s+1,c);
  s[3]=0;
  Send_string(s);
}


//===================================================
/* send newline and return characters via SCI */
//===================================================
void Send_newline(void)
{
  Send_string("\r\n");
}

//===================================================
/* read a character via SCI */
//===================================================
muint8 SCI_read(void)
{
  muint8 rc;
 
  while ( !(SCI1S1 & 0x20)); // wait for data to arrive
	
  rc = SCI1S1; // dummy read to clear flags
  rc = SCI1D; // data read
  
  return rc;
}

//=============================================
void SCI_write_char(char data_out)
{
  while (!(SCI1S1 & 0x80));  // wait for output buffer empty
    SCI1D = data_out;
}

//=============================================

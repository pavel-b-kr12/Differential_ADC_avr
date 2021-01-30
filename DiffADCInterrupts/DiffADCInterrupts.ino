// David Pilling August 2017
#include <stdarg.h>

void pr(char *fmt, ... )
{
  char buf[128]; // resulting string limited to 128 chars

  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);

  Serial.println(buf);
}


//==================================================================
// Free run differential ADC read from interrupts
// exponential average with 1/(1<<ADCSHIFT) fraction
// 

#include "DifferentialADC.c"

//http://www.avrfreaks.net/forum/any-neat-sign-extension-tricks-10-bit-16-bit

volatile long adcvalue=0;

#define ADCSHIFT 10

ISR(ADC_vect)
{
 long temp;
 long temp2;
 temp=adcvalue-(adcvalue>>ADCSHIFT);
 temp2=(long)(ADCL | (ADCH << 8));
 temp2=(temp2 ^ 512)-512;
 adcvalue=temp+temp2;
}


void analogsetup()
{
  int val;

  pr("analog setup do three reads of Vcc to stabilise");
// Note have to do this before setting off interrupts

  analogReferenceDiff(0x1); // Vcc reference

  val = analogVoltageSetup();
  pr("Vcc %d", val);
  val = analogVoltageSetup();
  pr("Vcc %d", val);
  val = analogVoltageSetup();
  pr("Vcc %d", val);
  val = analogVoltageSetup();
  pr("Vcc %d", val);


  
  analogSetDiffCode(analogGetCode(A4, A5, 200));
  sbi(ADCSRA, ADEN);  // enable ADC
  sbi(ADCSRA, ADATE);  // enable auto trigger
  ADCSRB &= ~(_BV(ADTS3) | _BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0)); // free run mode
  ADCSRA |= (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0)); // divide by 128 = 9.6 KHz sample rate
  sbi(ADCSRA, ADIE); // enable ADC interrupt
  sbi(ADCSRA, ADSC); // boot
}

//==================================================================

void setup()
{
 Serial.begin(115200);
 while (!Serial);

 analogsetup();
}

void loop()
{
 int val;
 
 val=(adcvalue>>ADCSHIFT);

 pr("adcvalue %d",val);
 
 val=analogVoltageValue(val,10,200);

 pr("voltage*10 %d",val);

 delay(1000);
}


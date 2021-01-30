// David Pilling July 2017
//A1-A0 @ mega2560

#include "DifferentialADC.c"
#include <stdarg.h>

void pr(const char *fmt, ... )
{
  char buf[128]; // resulting string limited to 128 chars

 va_list args;
 va_start (args, fmt );
 vsnprintf(buf, 128, fmt, args);
 va_end (args);

 Serial.println(buf);
}

void setup() 
{
 Serial.begin(115200); 
 while(!Serial);
}

void loop() 
{
 int code;
 int vcc;

 vcc=analogVoltageSetup();

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
 code=analogGetCode(A1,A0,200);
#elif defined(__AVR_ATmega32U4__)
 code=analogGetCode(A4,A5,200);
#elif defined(__AVR_ATmega1284__) 
 code=analogGetCode(A1,A0,200);
#else
#error "undefined processor"
#endif

 int sval=analogSignedValue(analogReadDiffCode(code));
 pr("Vcc: %d;	MUX code: %x;	signed value: %d;	voltage: %d", vcc, code, sval, analogVoltageValue(sval,10,200) );

 //delay(100);
}


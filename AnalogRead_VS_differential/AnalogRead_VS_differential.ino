#define GAIN	1 //seems like correct work with: 1|10|200 (10 is for 500mV, 200 is for 20mV)
//test result: HW-108A w 5mm magnet: 200 for >1cm, 10 > 0.5cm (closer than this gain cause saturation )

#define tab	Serial.print("\t");
#include "DifferentialADC.c"

int code;

void setup() 
{
 Serial.begin(115200); 
 while(!Serial);
 
 //int vcc=analogVoltageSetup();
 
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	code=analogGetCode(A1,A0,GAIN);
#elif defined(__AVR_ATmega32U4__)
	code=analogGetCode(A4,A5,GAIN);
#elif defined(__AVR_ATmega1284__) 
	code=analogGetCode(A1,A0,GAIN);
#else
#error "undefined processor"
#endif
}

void loop() 
{

	int a_diff_signed =analogSignedValue(analogReadDiffCode(code));
	int v=analogVoltageValue(a_diff_signed,10,GAIN);

	Serial.print(a_diff_signed); tab
	//Serial.print("voltage:"); tab
	Serial.print(v); tab
	
	//int a0=analogRead(A0);
	//int a1=analogRead(A1);
	int a2=analogRead(A2);
	int a3=analogRead(A3);
	//Serial.print(a0); tab
	//Serial.print(a1); tab
	//Serial.print(a2); tab
	//Serial.print(a3); tab
	Serial.print(a2-a3); tab
	Serial.println();
	//delay(10);
}

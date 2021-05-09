// Jae Park
// TER4MP with Mr. Wong
// 2021-05-01
// Sumo Bot

#include <LiquidCrystal.h>

//-------------Global Variables-------------//


// Pin Variables
const int motorA1=0;
const int motorA2=0;

const int motorB1=0;
const int motorB2=0;


const int DIP1 = 11;
const int DIP2 = 10; 
const int LDR1 = A4;
const int LDR2 = A5;

const int DISTANCESENSOR = 5;


LiquidCrystal lcd(13, 12, 14, 15, 16, 17); 	// lcd display object



void setup()
{
	Serial.begin(9600); 	// serial for debugging
 	
  	// initialize DIP switch
  	pinMode(DIP1, INPUT);
 	pinMode(DIP2, INPUT);
  	lcd.begin (16,2);
}

void loop()
{
	delay(100);
}

// Jae Park
// TER4MP with Mr. Wong
// 2021-05-01
// Sumo Bot

#include <LiquidCrystal.h>

//-------------Global Variables-------------//
const int LEFT = 1;
const int RIGHT = 2;
const int FORWARD = 3;


// Pin Variables
const int motorA1 = 8;
const int motorA2 = 9;
const int motorB1 = 6;
const int motorB2 = 7;

const int DIP1 = 11;
const int DIP2 = 10; 

const int LDR1 = A4;	// front LDR
const int LDR2 = A5;	// back LDR

int LDR1_reading, LDR2_reading;

const int THRESHOLD = 200; 	// light threshold for black arena tape
const int TURN_90_MILLIS = 500;
const int ROTATE_180_MILLIS = 1000;

const int DISTANCESENSOR = 5;
long distance, converted;
bool turn = false;		


LiquidCrystal lcd(13, 12, 14, 15, 16, 17); 	// lcd display object

void checkLDR(){
  	LDR1_reading = analogRead(LDR1);
  	LDR2_reading = analogRead(LDR2);
  	// if front LDR is on black tape, go backwards to stay in arena
  	if (LDR1_reading > THRESHOLD || LDR2_reading > THRESHOLD){
		turn = true;		
    }
}

// This method pings the distance sensor and returns the 
// distance of the object infront of the Sumo Bot.
void getDistance(){  	
  	// Get input
 	pinMode(DISTANCESENSOR, OUTPUT);
  	digitalWrite(DISTANCESENSOR, LOW);
  	delayMicroseconds(2);
  	digitalWrite(DISTANCESENSOR, HIGH); 
  	delayMicroseconds(10);
  	digitalWrite(DISTANCESENSOR, LOW);
  	pinMode(DISTANCESENSOR, INPUT);
  	
  	// Convert input into distance in cm
  	distance = pulseIn(DISTANCESENSOR, HIGH);
  	converted = distance /29; 	
}

// This method checks the input states of the DIP switches
void checkDIP(){
	int num1 = digitalRead(DIP1);
  	int num2 = digitalRead(DIP2);
  	// TODO: add turning time and figure out how to do 180 turn
    if (num1 == 1 && num2==1){		//back to back
    	turn(RIGHT, TURNSPEED);
    }else if (num1 == 0 && num2 == 1){	// side by side (same dir)
    	turn(LEFT, ); 
    }else if (num1 == 1 && num2 == 0){	// side by side (opp dir)
    	turn(RIGHT, ROTATE_90_MILLIS);
    }
}


// This method updates the display on the SumoBot screen
void updateDisplay(){
  	lcd.clear();
	lcd.setCursor(0, 0);
  	lcd.print("Distance:");
  	lcd.print(converted);
  	lcd.print("cm");
  	lcd.setCursor(0, 1);	
  	lcd.print("LDR1:");
  	lcd.print(LDR1_reading);
  	lcd.print("LDR2:");
  	lcd.print(LDR2_reading);
}

void setMotion(int direction, int speed){
    switch (direction){
		case LEFT:
            analogWrite(motorA1, speed);
            analogWrite(motorA2, 0);
            analogWrite(motorB1, 0);
            analogWrite(motorB2, speed);
      		break;
      	case RIGHT:
      		analogWrite(motorA1, 0);
            analogWrite(motorA2, speed);
            analogWrite(motorB1, speed);
            analogWrite(motorB2, 0);
      		break;
      	case FORWARD:
      		analogWrite(motorA1, 0);
            analogWrite(motorA2, speed);
            analogWrite(motorB1, 0);
            analogWrite(motorB2, speed);
			break;
    }// TODO: switch the analog wires to tilda ones

}

void setup(){
	Serial.begin(9600); 	// serial for debugging
  
  	// initialize DIP switch
  	pinMode(DIP1, INPUT);
 	pinMode(DIP2, INPUT);
  
  	pinMode(motorA1, OUTPUT);
  	pinMode(motorA2, OUTPUT);
  	pinMode(motorB1, OUTPUT);
  	pinMode(motorB2, OUTPUT);
  
  	
  	lcd.begin (16,2);
  
  	checkDIP();
	// TODO add time shit here for initial turning

}


void loop(){
  	unsigned long currentTime = millis();
  
  	getDistance();
  	checkLDR();
  	// Seeing an enemy and attacking takes priority
  	if (converted < DISTANCE_RANGE){
  		setMotion(FORWARD, ROTATE_90_MILLIS);
    } else if (turn){
		setMotion(RIGHT, ROTATE_90_MILLIS);	
      	//TODO: add time control to stop turning
    } else if (){
     	//TODO: NOT ATTACKING GOING FORWARD
    }
    
  	updateDisplay();


    
	delay(100);
}

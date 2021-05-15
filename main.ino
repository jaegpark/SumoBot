// Jae Park
// TER4MP with Mr. Wong
// 2021-05-01
// Sumo Bot

#include <LiquidCrystal.h>

//----------------Constants-----------------//
const int LEFT = 1;
const int RIGHT = 2;
const int FORWARD = 3;
const int THRESHOLD = 200; 					// light threshold for black arena tape
const int ROTATE_90_MILLIS = 1000;
const int ROTATE_180_MILLIS = 2000;
const int NORMAL_SPEED = 100;
const int FAST_SPEED = 200;
const int DISTANCE_RANGE = 50;

//-------------Global Variables-------------//

// Pin Variables
const int motorA1 = 9;
const int motorA2 = 10;
const int motorB1 = 3;
const int motorB2 = 6;
const int DIP1 = 11;
const int DIP2 = 8;
const int LDR1 = A4;
const int LDR2 = A5;
const int DISTANCESENSOR = 5;

// Helper variables
int LDR1_reading, LDR2_reading;
long distance;
bool turn = false;		
unsigned long curTime, exitTime;


LiquidCrystal lcd(13, 12, 14, 15, 16, 17); 	// lcd display object

/* 
  This method checks the LDR's and sets turn 
  status to true if arena boundary is detected.
*/
void checkLDR(){
  	LDR1_reading = analogRead(LDR1);
  	LDR2_reading = analogRead(LDR2);
  	// if front LDR is on white tape, go backwards to stay in arena
  	if (LDR1_reading > THRESHOLD || LDR2_reading > THRESHOLD){
		turn = true;		
    }
}

/*
  This method pings the distance sensor and finds the 
  distance of the object infront of the Sumo Bot.
*/
void getDistance(){  	
  	// Get input
 	pinMode(DISTANCESENSOR, OUTPUT);
  	digitalWrite(DISTANCESENSOR, LOW);
  	delayMicroseconds(2);
  	digitalWrite(DISTANCESENSOR, HIGH); 
  	delayMicroseconds(5);
  	digitalWrite(DISTANCESENSOR, LOW);
  	pinMode(DISTANCESENSOR, INPUT);
  	
  	// Convert input into distance in cm
  	distance = pulseIn(DISTANCESENSOR, HIGH)/58;
}

/*
  This method checks the initial input states
  of the DIP switches, and determines 
  the initial motion. 
*/ 
void checkDIP(){
	int num1 = digitalRead(DIP1);
  	int num2 = digitalRead(DIP2);
    if (num1 == 1 && num2==1){		// back to back
    	setMotion(RIGHT, NORMAL_SPEED);
      	exitTime = millis() + ROTATE_180_MILLIS;
    }else if (num1 == 0 && num2 == 1){	// side by side (same dir)
    	setMotion(LEFT, NORMAL_SPEED); 
      	exitTime = millis() + ROTATE_90_MILLIS;
    }else if (num1 == 1 && num2 == 0){	// side by side (opp dir)
    	setMotion(RIGHT, NORMAL_SPEED);
      	exitTime = millis() + ROTATE_90_MILLIS;
    }
}

/*
  This method updates the display on the SumoBot screen
*/ 
void updateDisplay(){
  	lcd.clear();
	lcd.setCursor(0, 0);
  	lcd.print("Distance:");
  	lcd.print(distance);
  	lcd.print("cm");
  	lcd.setCursor(0, 1);	
  	lcd.print("LDR1:");
  	lcd.print(LDR1_reading);
  	lcd.print("LDR2:");
  	lcd.print(LDR2_reading);
}

/*
  This method sets the motion of the sumobot motors
  and takes in direction and speed it should use.
*/
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
    }
}

/*
  Initializes setup of components, and adjusts the 
  orientation of the robot at the beginning of the fight.
*/
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
}


void loop(){
  	curTime = millis();
  	getDistance();
  	checkLDR();
  	// Seeing an enemy and attacking takes priority
  	if (distance < DISTANCE_RANGE){
  		setMotion(FORWARD, FAST_SPEED);
    } else if (turn){	// If turn needed (exiting arena), turn 90 degrees.
		setMotion(RIGHT, NORMAL_SPEED);	 
      	exitTime = curTime + ROTATE_90_MILLIS;
      	turn = false;
    } else if (curTime > exitTime){	// If turn time complete, go forward 
     	setMotion(FORWARD, NORMAL_SPEED);
    }

  	updateDisplay();
	delay(100);
}

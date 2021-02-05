/*
 
Stepper-based rotating stand controlled by serial dialogue

Circuit and comments: 
See http://www.cesarebrizio.it/Arduino/Turntable.html
Circuit is as illustrated here:
https://www.tdegypt.com/wp-content/uploads/2017/08/stepper-motor-wiring.png
the only exception being that the sketch uses digital outputs 4 - 5 - 6 - 7
while the Fritzing diagram 
 
 created 23 Dec 2017
 modified ----
 by Cesare Brizio
 
This example code is in the public domain.

This sketch controls a rotating platform based on a stepper motor.
Stepper model is 28BYJ48, controlled via the X113647 Stepper Motor 
Driver (based on the ULN2003A IC) normally purchased with the stepper.  

The platform can rotate in both directions and is controlled via Serial Monitor 
or Tera Term.
The following commands are implemented:

R = Rotate Clockwise continuously (meaning: for several turns)
r = Rotate for 360deg; clockwise
L = Rotate Counter-clockwise continuously (meaning: for several turns)
l = Rotate for 360deg; counter-clockwise
S = Stop rotating
 
Sources of information:
Small stepper control: http://arduino-info.wikispaces.com/SmallSteppers

*/

/*-----( Import needed libraries )-----*/
#include <AccelStepper.h>

/*-----( Declare Constants and Pin Numbers )-----*/
/* NEVER PUT ; AFTER A #define statement!!!! */
// motor pins
#define motorPin5  6     // Blue   - 28BYJ-48 pin 1
#define motorPin6  7     // Pink   - 28BYJ-48 pin 2
#define motorPin7  8     // Yellow - 28BYJ-48 pin 3
#define motorPin8  9   // Orange - 28BYJ-48 pin 4
#define motorPin1  10     // Blue   - 28BYJ-48 pin 1
#define motorPin2  11     // Pink   - 28BYJ-48 pin 2
#define motorPin3  12     // Yellow - 28BYJ-48 pin 3
#define motorPin4  13   // Orange - 28BYJ-48 pin 4
const int micPin  = A0;

int micVal  = 0;

int motorSpeed = 1900;

unsigned long starttime, endtime;
const long interval = 20000;
bool booly = false;

/*-----( Objects for stepper control )-----*/
// Set up the stepper as 4 wire bipolar on pin 4,5,6,7
// NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ48
AccelStepper stepper(4,motorPin1,motorPin3,motorPin2,motorPin4); 
AccelStepper stepper2(4,motorPin5,motorPin6,motorPin7,motorPin8);

void setup()
{
  Serial.begin(9600);// Advisable setting to avoid that pulses from Arduino
                           

  Serial.println(F("Initialize System"));
  //Init Microphone
  pinMode(micPin, INPUT);
                             // are too quick to be decoded
  stepper.setMaxSpeed(2000);
  stepper.setSpeed(-motorSpeed); // Advisable setting to avoid that pulses from Arduino
                                // are too quick to be decoded
  stepper2.setMaxSpeed(2000);
  stepper2.setSpeed(motorSpeed);
}

void loop()
{

    micVal = analogRead(micPin);
    Serial.print(F("mic val "));
    Serial.println(micVal);
    if (micVal > 200) {
      booly = true;
      starttime = millis();
      endtime = starttime;      
    }
    if(booly){
      if((endtime - starttime) <=interval){
        runSteppers();
        endtime = millis();
      }
      else{
        booly = false;
      }
    }
}
void runSteppers(){
  stepper2.runSpeed();
  stepper.runSpeed();
}

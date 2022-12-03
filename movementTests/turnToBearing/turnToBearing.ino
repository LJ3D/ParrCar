// Stuff for compass:
#include <Wire.h>
#include <QMC5883LCompass.h>
// My class for ensuring I dont explode anything:
#include "safeMotorControl.h" 


// !!!!!!!!!!!! DONT MESS UP THE PIN NUMBERS !!!!!!!!!!!!
#define LMF_PIN_MACRO 4 // Left Motor Forward
#define LMR_PIN_MACRO 16 // Left Motor Reverse
#define RMF_PIN_MACRO 5 // Right Motor Forward
#define RMR_PIN_MACRO 17 // Right Motor Reverse
#define triggerPin 25 // Trigger pin for ultrasonic sensor
#define echoPin 26 // Echo pin for ultrasonic sensor
#define compassSDA 21 // SDA pin for compass
#define compassSCL 22 // SCL pin for compass
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

QMC5883LCompass compass;
safeMotorControl mc(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO);

#define withinTolerance(x, y, tolerance) (abs(x - y) <= tolerance)

void setup(){
  mc.stop(); // Ensure motors arent spinning
  // Distance sensor pin setup:
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  compass.init();
  Serial.begin(115200); 
}

void loop() {
  // Read the compass:
  compass.read();
  int heading = compass.getAzimuth();
  // Now try to turn to the bearing 0 (with 2 degree tolerance):
  if(withinTolerance(heading, 0, 2)){
    mc.stop();
  } else if(heading > 180){
    mc.turnRight();
  } else {
    mc.turnLeft();
  }
}

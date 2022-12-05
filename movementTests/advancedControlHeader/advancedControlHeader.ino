// Stuff for compass:
#include <Wire.h>
#include <QMC5883LCompass.h>
// My class for ensuring I dont explode anything:
#include "safeMotorControlPWM.h"


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
safeMotorControlPWM mc(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO);


#define HIGH_SPEED 255
#define LOW_SPEED 175

#define withinTolerance(x, y, tolerance) (abs(x - y) <= tolerance)

void setup(){
  mc.stop();
  compass.init();
}

void loop(){
  compass.read();
  int heading = compass.getAzimuth();
  if(withinTolerance(heading, 0, 2)){
    mc.driveForward(HIGH_SPEED);
    delay(500);
  }else if(heading > 180){
    mc.turnRight(heading>350?LOW_SPEED:HIGH_SPEED);
  }else{
    mc.turnLeft(heading<10?LOW_SPEED:HIGH_SPEED);
  }
}

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


safeMotorControlPWM mc(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO);

void setup() {
  mc.stop();
}

void loop() {
  mc.driveForward(255);
  delay(1000);
  mc.driveForward(200);
  delay(1000);
  mc.driveForward(128);
  delay(1000);
}

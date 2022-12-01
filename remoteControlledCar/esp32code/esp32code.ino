// Wifi libs:
#include <WiFi.h>
#include <WiFiServer.h>
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

// Some less critical macros:
const char* AP_SSID = "IFTY_CANT_CODE"; // SSID for wifi
const char* AP_PASSWORD = "123456789"; // Password for wifi
#define SERVER_PORT 52727 // The port the server is listening on
#define MAX_PACKET_SIZE 255 // Max packet size - 255 is overkill for simple remote control commands (2 bytes would be enough)


// Some class objects:
safeMotorControl mc(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO); // Create a motor controller object - ALWAYS CHECK THE PINS !!!
QMC5883LCompass compass;
WiFiServer serv(SERVER_PORT);




// This function returns a distance measurement in cm from the ultrasonic sensor
int getDistance(){
  long duration, distance;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  return distance;
}

// Takes a command (char) and returns an integer representing either success/failure or a value (sensor reading)
int processCommand(char cmd, WiFiClient c){
  if(cmd == 'W' || cmd == 'w'){
    mc.driveForward();
    c.write("OK\n");
    return 1; // Success
  }
  else if(cmd == 'S' || cmd == 's'){
    mc.driveBackward();
    c.write("OK\n");
    return 1; // Success
  }
  else if(cmd == 'A' || cmd == 'a'){
    mc.turnLeft();
    c.write("OK\n");
    return 1; // Success
  }
  else if(cmd == 'D' || cmd == 'd'){
    mc.turnRight();
    c.write("OK\n");
    return 1; // Success
  }
  else if(cmd == 'Q' || cmd == 'q'){
    mc.stop();
    c.write("OK\n");
    return 1; // Success
  }
  else if(cmd == '1'){
    int distance = getDistance();
    char distanceString[11];
    sprintf(distanceString, "%d\n", distance);
    c.write(distanceString);
    return distance; // Return the distance
  }
  else if(cmd == '2'){
    compass.read();
    int compreading = compass.getX();
    char compStr[11];
    sprintf(compStr, "%d\n", compreading);
    c.write(compStr);
    return compreading;
  }
  else if(cmd == '3'){
    compass.read();
    int compreading = compass.getY();
    char compStr[11];
    sprintf(compStr, "%d\n", compreading);
    c.write(compStr);
    return compreading;
  }
  else if(cmd == '4'){
    compass.read();
    int compreading = compass.getZ();
    char compStr[11];
    sprintf(compStr, "%d\n", compreading);
    c.write(compStr);
    return compreading;
  }
  return 0; // Failure
}

// The setup function runs once when you press reset or power the board
void setup(){
  // Distance sensor pin setup:
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  compass.init();
  mc.stop(); // Ensure motors arent spinning
  Serial.begin(115200); 
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_SSID, AP_PASSWORD); // Start the wifi
  IPAddress IP = WiFi.softAPIP(); // Get the IP
  Serial.print("AP IP address: ");
  serv.begin(); // Start the server
}

// The main loop (runs forever after setup)
void loop(){
  WiFiClient c = serv.available(); // Check if a client is connected
  while(c.connected()){
    if(c.available()){
      char cmd[MAX_PACKET_SIZE];
      c.read((uint8_t*)cmd, MAX_PACKET_SIZE);
      Serial.print(cmd);
      Serial.print(" -> ");
      Serial.println(processCommand(cmd[0], c));
      // Flush the buffer:
      c.flush();
    }
  }
  Serial.println("!!! No client connected !!!");
  delay(5000);
}

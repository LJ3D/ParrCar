// Wifi libs:
#include <WiFi.h>
#include <WiFiMulti.h>

// My class for ensuring I dont explode anything:
#include "safeMotorControl.h" 


// !!!!!!!!!!!! DONT MESS UP THE PIN NUMBERS !!!!!!!!!!!!
#define LMF_PIN_MACRO 4 // Left Motor Forward
#define LMR_PIN_MACRO 16 // Left Motor Reverse
#define RMF_PIN_MACRO 5 // Right Motor Forward
#define RMR_PIN_MACRO 17 // Right Motor Reverse
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Some less critical macros:
#define REMOTE_IP "192.168.1.140" // The address of the computer running the server
#define REMOTE_PORT 52727 // The port the server is listening on
#include "wifiInfo.h" // This info is not included in the repo for obvious reasons ;) SSID + PASS. Create your own file with this info in it. (#define WAP_SSID ____ and #define WAP_PASS ____)
#define MAX_PACKET_SIZE 255 // Max packet size - 255 is overkill for simple remote control commands (2 bytes would be enough)


// Some class objects:
safeMotorControl mc(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO); // Create a motor controller object - ALWAYS CHECK THE PINS !!!
WiFiMulti wm; // Dont really need to use wifimulti since there is only one network, but theres no downside to it


// Set up the serial and wifi:
void setup(){
  mc.stop(); // Ensure motors arent spinning
  Serial.begin(115200); 
  WiFi.mode(WIFI_STA); // Fixes laggy wifi i think
  wm.addAP(WAP_SSID, WAP_PASS); // Add the wifi network to the list of networks to connect to (there is only one for now)
  while(wm.run() != WL_CONNECTED){ // While we are not connected to the network, keep trying to connect
    Serial.print(".");
    delay(100);
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  delay(500); // A little delay to make sure everything is ready cant hurt
}

// The main loop:
void loop(){
  // Let the serial port know where we are connecting to
  Serial.print("Connecting to ");
  Serial.print(REMOTE_IP);
  Serial.print(":");
  Serial.println(REMOTE_PORT);
  // Create a client and connect to the server:
  WiFiClient c;
  if(!c.connect(REMOTE_IP, REMOTE_PORT)){ // If we cant connect, wait a bit and try again
    Serial.println("Connection failed");
    Serial.println("Retrying in 5 seconds");
    delay(5000);
    return;
  }
  // Main loop while connected:
  Serial.println("Connected");
  while(c.connected()){
    // Read data from the connection:
    char cmd[MAX_PACKET_SIZE];
    c.read((uint8_t*)cmd, MAX_PACKET_SIZE);
    // Let the serial port know what we got:
    Serial.print("Received command: ");
    Serial.println(cmd);
    // Simple switch for the commands:
    switch(cmd[0]){ // The first byte is the command - any other bytes received are ignored
      case 'w':
      case 'W': // Not case-sensitive :)
        mc.driveForward();
        break;
      case 's':
      case 'S':
        mc.driveBackward();
        break;
      case 'a':
      case 'A':
        mc.turnLeft();
        break;
      case 'd':
      case 'D':
        mc.turnRight();
        break;
      case 'q':
      case 'Q':
        mc.stop();
        break;
      default:
        Serial.println("Invalid command");
        break;
    }
  }
  Serial.println("!!! Disconnected from server !!!");
}
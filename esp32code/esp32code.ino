#include <WiFi.h>
#include <WiFiMulti.h>

#include "safeMotorControl.h"

// VERY IMPORTANT MOTOR PIN NUMBERS - DONT FUCK THEM UP
#define LMF_PIN_MACRO 4
#define LMR_PIN_MACRO 16
#define RMF_PIN_MACRO 5
#define RMR_PIN_MACRO 17

// Less important stuff - Wifi stuffs
#define REMOTE_IP "192.168.1.140"
#define REMOTE_PORT 52727
#include "wifiInfo.h" // This info is not included in the repo for obvious reasons ;) SSID + PASS
#define MAX_PACKET_SIZE 255

safeMotorControl mc(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO);

WiFiMulti wm;

void setup(){
  mc.stop(); // Ensure motors arent spinning
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // Fixes laggy wifi i think
  // Ensure TCP is not delayed:
  wm.addAP(WAP_SSID, WAP_PASS);
  while(wm.run() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  delay(500);
}

void loop(){
  Serial.print("Connecting to ");
  Serial.print(REMOTE_IP);
  Serial.print(":");
  Serial.println(REMOTE_PORT);
  WiFiClient c;
  if(!c.connect(REMOTE_IP, REMOTE_PORT)){
    Serial.println("Connection failed");
    Serial.println("Retrying in 5 seconds");
    delay(5000);
    return;
  }
  Serial.println("Connected");
  while(c.connected()){
    // Listen for a command
    char cmd[MAX_PACKET_SIZE];
    c.read((uint8_t*)cmd, MAX_PACKET_SIZE);
    Serial.print("Received command: ");
    Serial.println(cmd);
    // Simple switch for the commands
    switch(cmd[0]){
      case 'w':
        mc.driveForward();
        break;
      case 's':
        mc.driveBackward();
        break;
      case 'a':
        mc.turnLeft();
        break;
      case 'd':
        mc.turnRight();
        break;
      case 'q':
        mc.stop();
        break;
      default:
        Serial.println("Invalid command");
        break;
    }
  }
  Serial.println("!!! Disconnected");
}
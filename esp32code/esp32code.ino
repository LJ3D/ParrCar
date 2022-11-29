#include <WiFi.h>
#include <WiFiMulti.h>

#include "safeMotorControl.h"

// VERY IMPORTANT MOTOR PIN NUMBERS - DONT FUCK THEM UP
#define LMF_PIN_MACRO 4
#define LMR_PIN_MACRO 16
#define RMF_PIN_MACRO 5
#define RMR_PIN_MACRO 17

// Less important stuff - Wifi stuffs
#define REMOTE_IP "192.168.159.25"
#define REMOTE_PORT 52727
#define WAP_SSID "A"
#define WAP_PASS "AAAAAAAA"
#define MAX_PACKET_SIZE 255

safeMotorControl mc(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO);

WiFiMulti wm;

void setup(){
  mc.stop(); // Ensure motors arent spinning
  Serial.begin(115200);
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
    // Shout among us sussy balls at the server
    c.println("among us sussy balls");
    delay(2000);
  }

}
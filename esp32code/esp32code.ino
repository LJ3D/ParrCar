#include <WiFi.h>
#include <WiFiServer.h>

#include "safeMotorControl.h"

// VERY IMPORTANT MOTOR PIN NUMBERS - DONT FUCK THEM UP
#define LMF_PIN_MACRO 4
#define LMR_PIN_MACRO 16
#define RMF_PIN_MACRO 5
#define RMR_PIN_MACRO 17

// Less important stuff - Wifi stuffs
#define PORT 52727
#define WAP_SSID "A"
#define WAP_PASS "AAAAAAAA"
#define MAX_PACKET_SIZE 255

safeMotorControl motorControl(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO);

WiFiServer server(PORT);
WiFiClient remoteClient;

void setup(){
  motorControl.stop(); // Absolutely ensure that none of the motor pins are active (dont really need to do this, the class does it itself)
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(WAP_SSID, WAP_PASS);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  // Set stuff up to listen for incoming data:
  server.begin();
  Serial.println("Connected and set up (probably)");
  Serial.println(WiFi.localIP());
}




void checkForConnections(){
  if(server.hasClient()){
    if(remoteClient.connected()){
      Serial.println("Rejected connection - already have connected client");
      server.available().stop();
    }else{
      Serial.println("Connection accepted");
      remoteClient = server.available();
    }
  }
}

void echoOnce(){
  uint8_t receiveBuffer[32];
  int received = remoteClient.read(receiveBuffer, sizeof(receiveBuffer));
  remoteClient.write(receiveBuffer, received);
}

void mainloop(){
  // Drive forward for a bit
  motorControl.driveForward();
  delay(500);
  motorControl.stop();
  // Send wifi data
  remoteClient.write("AMONG US");
  delay(500);
}

void loop() {
  Serial.println("Checking for connection...");
  checkForConnections();
  if(remoteClient.connected()){
    mainloop();
  }
  delay(50);
}

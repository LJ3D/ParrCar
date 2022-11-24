#include <WiFi.h>
#include <WiFiUdp.h>

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

safeMotorControl motorControl(LMF_PIN_MACRO, LMR_PIN_MACRO, RMF_PIN_MACRO, RMR_PIN_MACRO);
WiFiUDP Udp;

void setup(){
  motorControl.stop(); // Absolutely ensure that none of the motor pins are active (dont really need to do this, the class does it itself)
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(WAP_SSID, WAP_PASS);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Beginning UDP port on ");
  Serial.println(PORT);
  Udp.begin(PORT);
}

void loop() {
  motorControl.driveForward();
  delay(1000);
  motorControl.driveBackward();
  delay(1000);
  motorControl.turnLeft();
  delay(1000);
  motorControl.turnRight();
  delay(1000);
  motorControl.stop();
  delay(1000);
  // Send test data over wifi to 192.168.159.25
  Udp.beginPacket("192.168.159.25", PORT);
  size_t x = Udp.println("AMONG US");
  Serial.println(x);
  Udp.endPacket();
}

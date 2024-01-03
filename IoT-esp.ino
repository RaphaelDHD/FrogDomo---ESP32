#include "controller.h"
#include "FeatherShieldPinouts.h"

#include <WiFi.h>
#include <HTTPClient.h>


const char* WIFI_SSID = "Livebox-7120";
const char* WIFI_PASSWORD = "z3RRaFWecKPghusCbc";

String HOST_NAME   = "http://YOUR_DOMAIN.com"; // CHANGE IT
String PATH_NAME   = "/products/arduino";      // CHANGE IT
String queryString = "temperature=26&humidity=70"; // OPTIONAL



ServoController servo;
FanController fan;
LedController led(D4,D5);
OpeningController openingSensor;
CodeController code;
WifiController wifi;

void setup()
{
  Serial.begin(115200); // debug
  servo.attach(A0);
  fan.attach(A5);
  openingSensor.attach(D2);
  code.attach(4);
  wifi.connect(WIFI_SSID, WIFI_PASSWORD);
}


void loop()
{
  bool test = code.readValue();
  if (test)
    openingSensor.reverseActivated();
  bool alarmTriggered = openingSensor.readValue();
  if (alarmTriggered) {
    Serial.println("INTRUS A L'AIDE");
  }
  delay(100);
}


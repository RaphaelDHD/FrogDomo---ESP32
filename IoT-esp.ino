#include "controller.h"
#include "FeatherShieldPinouts.h"

ServoController servo;
FanController fan;
LedController led(D4,D5);
OpeningController openingSensor;
CodeController code;

void setup()
{
  Serial.begin(115200); // debug
  servo.attach(A0);
  fan.attach(A5);
  openingSensor.attach(D2);
  code.attach(4);
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


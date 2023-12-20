#include "controller.h"
#include "FeatherShieldPinouts.h"

ServoController servo;
FanController fan;
LedController led(D2,D3);


void setup()
{
  Serial.begin(115200);
  servo.attach(A0);
  fan.attach(A5);
}


void loop()
{
  led.setColor(255,0,0);
}

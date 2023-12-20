#include "controller.h"

void FanController::attach(int pin) {
  _pin = pin; 
  pinMode(pin, OUTPUT);
}

void FanController::setSpeed(int speed) {
  analogWrite(_pin,speed);
}
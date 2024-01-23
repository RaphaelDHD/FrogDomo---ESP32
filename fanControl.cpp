#include "controller.h"

void FanController::attach(int pin) {
  _pin = pin; 
  _active = false;
  pinMode(pin, OUTPUT);
}

void FanController::setSpeed(int speed) {
  analogWrite(_pin,speed);
}


void FanController::setActive(bool value) {
  _active = value;
}

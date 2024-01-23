#include "controller.h"

void OpeningController::attach(int pin) {
  _pin = pin; 
  pinMode(pin, INPUT_PULLUP);
  _active = false;
  isTriggered = false;
}

bool OpeningController::readValue() {
  if (_active) {
    isTriggered = digitalRead(_pin);
    return isTriggered;
  }
  return false;
}

void OpeningController::setActivated(bool value) {
  _active = value;
}

void OpeningController::reverseActivated() {
  _active = !_active;
}

bool OpeningController::getActive() {
  return _active;
}

bool OpeningController::GetIsTriggered(){
  return isTriggered;
}


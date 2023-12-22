#include "controller.h"

void OpeningController::attach(int pin) {
  _pin = pin; 
  pinMode(pin, INPUT_PULLUP);
  activated = false;
  isTriggered = false;
}

bool OpeningController::readValue() {
  if (activated) {
    isTriggered = digitalRead(_pin); 
    return isTriggered;
  }
  return false;
}

void OpeningController::setActivated(bool value) {
  activated = value;
}

void OpeningController::reverseActivated() {
  activated = !activated;
}


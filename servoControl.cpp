#include "controller.h"

void ServoController::attach(int pin) {
  servoMotor.attach(pin);  // attaches the servo on ESP32 pin
}

void ServoController::setRotation(int rotation) {
  servoMotor.write(rotation);
}
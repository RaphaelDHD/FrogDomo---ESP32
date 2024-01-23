#include "controller.h"

LedController::LedController(int clkpin, int datapin) : _led(clkpin, datapin, 1) {
}

void LedController::setColor(int r, int g, int b) {
  _led.setColorRGB(0,r,g,b);
}


void LedController::setActive(bool value) {
  _active = value;
}

void LedController::setColorFromHex(String hexColor) {
  if (hexColor.length() == 7 && hexColor[0] == '#') {
    int r = strtol(hexColor.substring(1, 3).c_str(), nullptr, 16);
    int g = strtol(hexColor.substring(3, 5).c_str(), nullptr, 16);
    int b = strtol(hexColor.substring(5, 7).c_str(), nullptr, 16);
    _led.setColorRGB(0, r, g, b);
  } else {
    Serial.println("Invalid hex color format");
  }
}
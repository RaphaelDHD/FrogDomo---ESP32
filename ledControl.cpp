#include "controller.h"

LedController::LedController(int clkpin, int datapin) : _led(clkpin, datapin, 1) {
}

void LedController::setColor(int r, int g, int b) {
  _led.setColorRGB(0,r,g,b);
}
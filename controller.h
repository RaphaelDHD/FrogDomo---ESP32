#include "Servo.h"
#include "ChainableLED.h"

class FanController {
  public:
    void attach(int pin);
    void setSpeed(int speed);
  private:
    int _pin;
};


class ServoController {
  public:
    void attach(int pin);
    void setRotation(int rotation);
  private:
    Servo servoMotor;
};

class LedController{
  public:
    LedController(int clkpin, int datapin);
    void setColor(int r, int g, int b);
    int value();
  private:
    ChainableLED _led;
};

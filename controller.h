#include <string>
#include "Servo.h"
#include "ChainableLED.h"
#include <WiFiClientSecure.h>

using namespace std;

class FanController {
public:
  void attach(int pin);
  void setSpeed(int speed);
private:
  int _pin;
};

class OpeningController {
public:
  void attach(int pin);
  bool readValue();
  void setActivated(bool value);
  void reverseActivated();
private:
  int _pin;
  bool activated;
  bool isTriggered;
};

class CodeController {
public:
  void attach(int size);
  bool readValue();
  bool checkCode();
private:
  int codeSize;
  int* code;  // Use int* for dynamic arrays
  void enterNumber(int value);
};

class ServoController {
public:
  void attach(int pin);
  void setRotation(int rotation);
private:
  Servo servoMotor;
};

class LedController {
public:
  LedController(int clkpin, int datapin);
  void setColor(int r, int g, int b);
  int value();
private:
  ChainableLED _led;
};

class WifiController {
public:
  void connect(const char* ssid, const char* password);
};

class ApiController {
public:
  void setAPIUrl(String apiUrl);
  bool login(String email, String password);
  void get();
private:
  String _apiUrl;
};

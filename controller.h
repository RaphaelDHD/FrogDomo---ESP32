#include <string>
#include "Servo.h"
#include "ChainableLED.h"

using namespace std;

struct UserInfo {
  int fanSpeed;
  bool fanActive;
  bool alarmActive;
  int portalValue;
  bool lightBulbActive;
  String lightBulbColor;
};



class FanController {
public:
  void attach(int pin);
  void setSpeed(int speed);
  void setActive(bool value);
private:
  int _pin;
  bool _active;
};

class OpeningController {
public:
  void attach(int pin);
  bool readValue();
  void setActivated(bool value);
  void reverseActivated();
  bool getActive();
  bool GetIsTriggered();
private:
  int _pin;
  bool _active;
  bool isTriggered;
};

class CodeController {
public:
  void attach(int size);
  bool readValue();
  bool checkCode();
private:
  int codeSize;
  int* code;
  void enterNumber(int value);
};

class ServoController {
public:
  void attach(int pin);
  void setRotation(int rotation);
private:
  Servo servoMotor;
  int _rotation = 0;
};

class LedController {
public:
  LedController(int clkpin, int datapin);
  void setColor(int r, int g, int b);
  int value();
  void setActive(bool value);
  void setColorFromHex(String hexColor);
private:
  ChainableLED _led;
  bool _active;
  int _red;
  int _green;
  int _blue;
};

class WifiController {
public:
  void connect(const char* ssid, const char* password);
};

class ApiController {
public:
  void setAPIUrl(String apiUrl);
  bool login(String email, String password);
  UserInfo get();
  void updateAlarm(bool activeValue, bool rungValue);
private:
  String _apiUrl;
  String _userId;
};

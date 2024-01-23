#include "controller.h"
#include "FeatherShieldPinouts.h"
#include "HTML.h"


#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char *WIFI_SSID = "PHONE_ESP";
const char *WIFI_PASSWORD = "esp32-iot";

AsyncWebServer server(80);

ServoController servo;
FanController fan;
LedController led(D4, D5);
OpeningController openingSensor;
CodeController code;
WifiController wifi;
ApiController api;

bool isAuthenticated = false;

UserInfo userInfo;

bool _alarmIsTriggered = NULL;
bool _alarmIsActive = NULL;

void manageInfo(UserInfo info) {
  // manage fan
  if (!info.fanActive) {
    fan.setSpeed(0);
  } else {
    fan.setSpeed(info.fanSpeed);
  }

  // manage servo
  servo.setRotation(info.portalValue);

  // manage light bulb
  if (!info.lightBulbActive) {
    led.setActive(false);
    led.setColor(0, 0, 0);
  } else {
    led.setActive(true);
    led.setColorFromHex(info.lightBulbColor);
  }
}



void setup() {
  Serial.begin(115200);  // debug
  servo.attach(A0);
  fan.attach(A5);
  openingSensor.attach(D2);
  code.attach(4);

  wifi.connect(WIFI_SSID, WIFI_PASSWORD);
  //api.setAPIUrl(API_URL);

  // Display the URL of the server
  Serial.println("Server started");
  Serial.print("To access the login page, open your web browser and go to: http://");
  Serial.print(WiFi.localIP());
  Serial.println(":80");


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", login_html);
  });

  server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("Received login request");

    // Get the values from the form
    String email = request->arg("email");
    String password = request->arg("password");
    String API_URL = request->arg("ip");
    api.setAPIUrl(API_URL);
    // Perform your custom authentication logic
    if (api.login(email, password)) {
      Serial.println("Authentication successful");
      request->redirect("/login-success");
      isAuthenticated = true;
    } else {
      Serial.println("Authentication failed");
      // Authentication failed, redirect to a login error page
      request->redirect("/login-error");
    }
  });

  server.on("/login-error", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", login_error_html);
  });

  server.on("/login-success", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", login_success_html);
  });

  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(401);
    Serial.println("Logout successful");
    isAuthenticated = false;
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", logout_html);
  });

  // Start server
  server.begin();
}

unsigned long lastOpeningSensorTime = 0;
unsigned long openingSensorInterval = 200;  // Set your desired interval in milliseconds

unsigned long lastApiGetTime = 0;
unsigned long apiGetInterval = 3000;  // Set your desired interval in milliseconds

void loop() {
  unsigned long currentMillis = millis();
  bool codeIsValid = code.readValue();
  if (codeIsValid) {
    openingSensor.reverseActivated();
  }

  if (isAuthenticated && currentMillis - lastOpeningSensorTime >= openingSensorInterval) {
    lastOpeningSensorTime = currentMillis;
    bool isOpeningSensorActive = openingSensor.getActive();
    bool isAlarmTriggered = openingSensor.readValue();
    Serial.print("Alarm Triggered: ");
    Serial.print(isAlarmTriggered);
    Serial.print(" | Opening Sensor Active: ");
    Serial.println(isOpeningSensorActive);
    if (isAlarmTriggered != _alarmIsTriggered || isOpeningSensorActive != _alarmIsActive) {
      api.updateAlarm(isOpeningSensorActive, isAlarmTriggered);
      _alarmIsTriggered = isAlarmTriggered;
      _alarmIsActive = isOpeningSensorActive;
    }

    if (currentMillis - lastApiGetTime >= apiGetInterval) {
      lastApiGetTime = currentMillis;
      userInfo = api.get();
      manageInfo(userInfo);
    }

    // Optional delay, adjust as needed
    delay(10);
  }
}

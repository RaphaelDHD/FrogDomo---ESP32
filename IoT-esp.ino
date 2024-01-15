#include "controller.h"
#include "FeatherShieldPinouts.h"
#include "HTML.h"

#include <WiFiClientSecure.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char *WIFI_SSID = "PHONE_ESP";
const char *WIFI_PASSWORD = "esp32-iot";

AsyncWebServer server(80);
WiFiClientSecure client;


ServoController servo;
FanController fan;
LedController led(D4, D5);
OpeningController openingSensor;
CodeController code;
WifiController wifi;
ApiController api;

bool isAuthenticated = false;

UserInfo userInfo;


void manageInfo(UserInfo info) {
  // manage fan
  Serial.println(info.fanActive);
  Serial.println(info.fanSpeed);
  if (!info.fanActive) {
    fan.setSpeed(0);
  } else {
    fan.setSpeed(info.fanSpeed);
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

void loop() {
  bool test = code.readValue();
  if (test)
    openingSensor.reverseActivated();
  bool alarmTriggered = openingSensor.readValue();
  if (alarmTriggered) {
    Serial.println("INTRUS A L'AIDE");
  }

  
  if (isAuthenticated) {
    userInfo = api.get();
    manageInfo(userInfo);
  }

  delay(3000);
}

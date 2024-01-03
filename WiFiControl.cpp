#include "controller.h"
#include <WiFi.h>
#include <HTTPClient.h>




void WifiController::connect(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}


bool WifiController::login(string mail, string password) {
}

void WifiController::setAPIUrl(string apiUrl) {
  _apiURL = apiUrl;
}

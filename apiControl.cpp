#include "controller.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>

// Adjust the size as needed based on the expected JSON payload size
DynamicJsonDocument doc(128);
char jsonOutput[128];
char loginUrl[128];

void ApiController::setAPIUrl(String apiUrl) {
  _apiUrl = apiUrl;
}

bool ApiController::login(String email, String password) {
  WiFiClient client;
  HTTPClient http;

  String loginUrl = _apiUrl + "users/login";

  Serial.println(loginUrl.c_str());

  http.begin(client, loginUrl);
  http.setTimeout(10000);

  http.addHeader("Content-Type", "application/json"); 

  JsonObject object = doc.to<JsonObject>();
  object["email"] = email;
  object["password"] = password;

  serializeJson(object, jsonOutput);

  int httpResponseCode = http.POST(jsonOutput);

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println("\nStatusCode: " + String(httpResponseCode));
    Serial.println("Server response: " + payload);
    http.end();
    if (httpResponseCode == 200) {
      return true;
    }
    return false;
  } else {
    Serial.println("Error in HTTP request");
    Serial.println(httpResponseCode);
    return false;
  }
}

void ApiController::get() {
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      

      http.begin(_apiUrl.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
}

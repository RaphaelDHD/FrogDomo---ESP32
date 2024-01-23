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
    Serial.println(payload);
    http.end();
    if (httpResponseCode == 200) {
      _userId = payload;
      return true;
    }
    return false;
  } else {
    Serial.println("Error in HTTP request");
    Serial.println(httpResponseCode);
    return false;
  }
}

UserInfo ApiController::get() {
  UserInfo userInfo;  // Create an instance of the struct

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String getUserIdInfo = _apiUrl + "users/" + _userId;
    //Serial.println(getUserIdInfo);
    http.begin(getUserIdInfo.c_str());

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      //Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);
      String payload = http.getString();
      // Serial.println(payload);

      // Parse JSON
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      // Extract values
      userInfo.fanSpeed = doc["fan"]["speed"];
      userInfo.fanActive = doc["fan"]["active"];
      userInfo.alarmActive = doc["alarm"]["active"];
      userInfo.portalValue = doc["portal"];
      userInfo.lightBulbActive = doc["light_bulb"]["active"];
      userInfo.lightBulbColor = doc["light_bulb"]["color"].as<String>();


    } else {
      //Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }

  return userInfo;  // Return the struct with extracted values
}

void ApiController::updateAlarm(bool activeValue, bool rungValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = _apiUrl + "users/" + _userId;
    http.begin(url);

    // Prepare the JSON body
    DynamicJsonDocument doc(1024);  // Adjust the size as needed
    JsonObject root = doc.to<JsonObject>();
    JsonObject alarmObject = root.createNestedObject("alarm");
    alarmObject["active"] = activeValue;
    alarmObject["rung"] = rungValue;

    String jsonOutput;
    serializeJson(root, jsonOutput);

    http.addHeader("Content-Type", "application/json");

    // Make the PUT request
    int httpResponseCode = http.PUT(jsonOutput);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);

    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
  }
}

/*
  ESP32 Virtual LED Controller
  Wokwi + Firebase Realtime Database

  Device: esp32_virtual_001
  LED 1: GPIO 26
  LED 2: GPIO 27

  Firebase:
  /devices/esp32_virtual_001/led1
  /devices/esp32_virtual_001/led2
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Wokwi
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6

// Firebase Realtime Database
#define FIREBASE_HOST "https://wokwi-f4741-default-rtdb.asia-southeast1.firebasedatabase.app"

// Device ID
#define DEVICE_ID "esp32_virtual_001"

// LED GPIO
#define LED1 26
#define LED2 27

bool led1State = false;
bool led2State = false;

unsigned long lastFirebaseRead = 0;
const unsigned long READ_INTERVAL = 1000;

// Firebase URL
String getFirebaseURL() {
  return String(FIREBASE_HOST) +
         "/devices/" DEVICE_ID ".json";
}

// Connect WiFi
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Read Firebase
void readFirebase() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Disconnected");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, getFirebaseURL());

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error && !doc.isNull()) {
      bool newLed1 = doc["led1"] | false;
      bool newLed2 = doc["led2"] | false;

      if (newLed1 != led1State) {
        led1State = newLed1;
        digitalWrite(LED1, led1State);

        Serial.printf("[LED 1] %s\n",
                      led1State ? "ON" : "OFF");
      }

      if (newLed2 != led2State) {
        led2State = newLed2;
        digitalWrite(LED2, led2State);

        Serial.printf("[LED 2] %s\n",
                      led2State ? "ON" : "OFF");
      }
    } else if (error) {
      Serial.print("[JSON] Error: ");
      Serial.println(error.c_str());
    }
  } else {
    Serial.printf("[Firebase] HTTP Error: %d\n", httpCode);

    if (httpCode > 0) {
      Serial.println(http.getString());
    }
  }

  http.end();
}

// Setup
void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  Serial.println("\nESP32 Firebase LED Controller");

  connectWiFi();

  readFirebase();
}

// Loop
void loop() {
  unsigned long now = millis();

  if (now - lastFirebaseRead >= READ_INTERVAL) {
    lastFirebaseRead = now;
    readFirebase();
  }

  delay(10);
}

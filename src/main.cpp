#include <Arduino.h>
#include <WiFi.h>

#define WIFI_SSID "Sahiths iphone"
#define WIFI_PASS "123456sb"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connecting to WiFi...");
}

bool connected = false;

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED && !connected) {
    Serial.println("Connected to WiFi");
    connected = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from WiFi");
    connected = false;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
  }
}

// #include <Arduino.h>
// #include <WiFiMulti.h>

// #define WIFI_SSID "Sahiths iphone"
// #define WIFI_PASS "123456sb"

// WiFiMulti wifimulti;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(921600);
//   pinMode(LED_BUILTIN, OUTPUT);

//   wifimulti.addAP(WIFI_SSID, WIFI_PASS);

//   while (wifimulti.run() != WL_CONNECTED) {
//     Serial.println("Connecting to WiFi...");
//     delay(1000);
//   }
//   Serial.println("Connected to WiFi");
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   delay(1000);
//   digitalWrite(LED_BUILTIN, wifimulti.run() == WL_CONNECTED ? HIGH : LOW);
//   Serial.println("loop print");
//   delay(1000);
//   digitalWrite(LED_BUILTIN, LOW);
// }
// }
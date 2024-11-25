#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define WIFI_SSID "Sahiths iphone" // modify this to the SSID of the base station Wi-Fi network
#define WIFI_PASS "123456sb" // modify this to the password of the base station Wi-Fi network
#define SERVER_IP "172.20.10.9" // modify this to the IP address of the server
#define PORT_NUM 1234
#define BAUD_RATE 921600

// #define DEVICE_ROLE_SERVER_IP
// #define DEVICE_ROLE_CLIENT_IP
// #define DEVICE_ROLE_SERVER_MAC
// #define DEVICE_ROLE_CLIENT_MAC

#ifdef DEVICE_ROLE_SERVER_IP

WiFiServer server(PORT_NUM);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\n');
        Serial.print("Received: ");
        Serial.println(request);
        client.print("Pong: ");
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}

#endif

#ifdef DEVICE_ROLE_CLIENT_IP

WiFiClient client;

void setup() {
  Serial.begin(BAUD_RATE);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to the server
  if (client.connect(SERVER_IP, PORT_NUM)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection to server failed");
    ESP.restart();
  }
}

void loop() {
  if (client.connected()) {
    client.println("Ping"); // Send Ping
    Serial.println("Sent: Ping");

    // Wait for response
    if (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.print("Received: ");
      Serial.println(response);
    }
  } else {
    Serial.println("Disconnected from server");
    delay(1000);
    ESP.restart(); // Reconnect
  }
  delay(1000); // Ping every second
}

#endif

#ifdef DEVICE_ROLE_SERVER_MAC

// Callback when data is received
void onReceive(const uint8_t *mac_addr, const uint8_t *data, int len) {
  char incomingData[32]; // Adjust size based on expected message length
  memcpy(incomingData, data, len);
  incomingData[len] = '\0';

  Serial.print("Received message: ");
  Serial.println(incomingData);

  // Respond with "Pong"
  const char *response = "Pong";
  esp_now_send(mac_addr, (uint8_t *)response, strlen(response));
}

void setup() {
  Serial.begin(BAUD_RATE);

  // Initialize WiFi in STA mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW Initialized");
  Serial.println("MAC Address: " + WiFi.macAddress());

  // Register callback
  esp_now_register_recv_cb(onReceive);
}

void loop() {
    // Nothing to do here; communication happens in the callback
}

#endif

#ifdef DEVICE_ROLE_CLIENT_MAC

// Receiver's MAC Address
uint8_t receiverMAC[] = {0x58, 0xCF, 0x79, 0x04, 0x3E, 0x9C}; // Replace with the receiver's MAC address

// Callback when message is sent
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Message Sent Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(BAUD_RATE);

  // Initialize WiFi in STA mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback
  esp_now_register_send_cb(onSent);

  // Add peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Send "Ping" to the receiver
  const char *message = "Ping";
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)message, strlen(message));

  Serial.println(result == ESP_OK ? "Ping sent" : "Ping failed");

  delay(1000); // Send a ping every second
}

#endif
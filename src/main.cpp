#include "Arduino.h"
#include "PubSubClient.h"
#include "WiFi.h"

const int led = 2;

int ledOn = 0;
int acOn = 0;

const char* wifi = "";
const char* wifiPassword = "";

const char* server = "smartbox.frandika.com";
const int port = 1883;

WiFiClient wifi_client;
PubSubClient client(wifi_client);

void setupWifi() {
  delay(10);

  Serial.print("Connecting to WiFi");
  WiFi.begin(wifi, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("Connected to WiFi!");
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "ac") == 0) {
    char msg[length + 1];
    memcpy(msg, payload, length);
    msg[length] = 0;

    if (strcmp(msg, "1") == 0) {
      Serial.println("AC ON");
      acOn = 1;
    } else {
      Serial.println("AC OFF");
      acOn = 0;
    }
  } else if (strcmp(topic, "led") == 0) {
    char msg[length + 1];
    memcpy(msg, payload, length);
    msg[length] = 0;

    if (strcmp(msg, "1") == 0) {
      Serial.println("LAMP ON");
      ledOn = 1;
    } else {
      Serial.println("LAMP OFF");
      ledOn = 0;
    }
  }

  digitalWrite(led, ledOn || acOn);

}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  setupWifi();

  client.setServer(server, port);
  client.connect("arduino");

  client.subscribe("ac");
  client.subscribe("led");
  client.setCallback(callback);
}

void loop() {
  client.loop();
}
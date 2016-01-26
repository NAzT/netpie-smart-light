#include <WiFiConnector.h>
#include <MqttConnector.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <EEPROM.h>



MqttConnector *mqtt;
WiFiConnector *wifi;
int state[2];
boolean current_state;
boolean _published_flag = false;
uint32_t prev = millis();


#define RELAY_01_PIN 2
#define RELAY_01_MASK 0b0000001

#define EEPROM_ADDR 0x00

#define MQTT_PORT        1883
#define PUBLISH_EVERY    5*1000 // every 15 seconds

/* comment ทิ้งถ้าไม่ส่ username/password */
//#define MQTT_USERNAME "GaBH7sxFDUEX0hl"
//#define MQTT_PASSWORD "YHK/C+Q2D4Juz9iPuHCrJL3IJLU="
//#define MQTT_CLIENT_ID "x6Xu0gY0nWyj4inu"

#define MQTT_HOST "gb.netpie.io"
#define MQTT_USERNAME "GaBH7sxFDUEX0hl"
#define MQTT_PASSWORD "pv1ycZPfj4285/E9SJCrKBvk/nI="
#define MQTT_CLIENT_ID "WlTwolbtXDaqQbWB"
#define MQTT_PREFIX "/NatWeerawan/gearname"

/* SENSOR INFO */
#define DEVICE_NAME "ESP01-SMART-LIGHT"
#define AUTHOR      "Nat Weerawan"
#define BOARD       "ESP-01"
#define SENSOR      "SSR"

/* WIFI INFO */
#define WIFI_SSID        ""
#define WIFI_PASSPHARSE  ""

#define SWITCH_PIN 0

#include "init_wifi.h"
#include "_receive.h"
#include "_publish.h"


String queue = "";

void init_hardware()
{
  EEPROM.begin(10);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("BEGIN");
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(RELAY_01_PIN, OUTPUT);

  byte val = EEPROM.read(EEPROM_ADDR);
  Serial.println(val, BIN);
  current_state =  val & RELAY_01_MASK;

  Serial.print("START STATE: ");
  Serial.println(current_state);

  digitalWrite(RELAY_01_PIN, current_state);

  //   digitalWrite(RELAY_02_PIN, val & RELAY_02_MASK);
  //  pinMode(DHTPIN, INPUT_PULLUP);
  //  pinMode(DHT_VCC, OUTPUT);
  //pinMode(DHT_GND, OUTPUT);
  ///  digitalWrite(DHT_VCC, HIGH);
  //  digitalWrite(DHT_GND, LOW);

}

void setup()
{
  init_hardware();
  wifi = init_wifi();
  mqtt = init_mqtt();

  wifi->connect();
}

void loop()
{
  mqtt->loop();
  prev = millis();

  while (digitalRead(SWITCH_PIN) == LOW) {
    mqtt->loop();
    if (millis() - prev > 70) {
      prev = millis();
      if (current_state == 0) {
        queue = "g";
        digitalWrite(SWITCH_PIN, HIGH);
      }
      else {
        digitalWrite(SWITCH_PIN, LOW);
        queue = "h";
      }
      Serial.println(String("HEAP: ") + ESP.getFreeHeap());
      mqtt->sync_pub(queue);
    }
    yield();
  }

}

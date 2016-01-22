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

#define RELAY_01_PIN 16
#define RELAY_01_MASK 0b0000001

#define EEPROM_ADDR 0x00

#define MQTT_PORT        1883
#define PUBLISH_EVERY    10*1000 // every 15 seconds

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
#define DEVICE_NAME "ESPRESSO-SMART-LIGHT"
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

uint32_t prev = millis();

void init_hardware()
{
  EEPROM.begin(10);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("BEGIN");
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(RELAY_01_PIN, OUTPUT);
  //  auto cb_fn = []() -> void {
  //    int _status = digitalRead(0);
  //    state[_status] = millis();
  //
  //    Serial.print(" CHANGE => ");
  //    Serial.println(_status);
  //
  //    if (_status == HIGH) {
  //      int val = state[1] - state[0];
  //      if (val > 50) {
  //        Serial.println(val);
  //        current_state = !current_state;
  //        digitalWrite(RELAY_01_PIN, current_state);
  //        if (current_state == true) {
  //          Serial.println("STATE: ON");
  //          EEPROM.write(EEPROM_ADDR, 'g');
  ////          EEPROM.commit();
  ////          mqtt->sync_pub(String("g"));
  //        }
  //        else {
  //          Serial.println("STATE: OFF");
  //          EEPROM.write(EEPROM_ADDR, 'h');
  ////          EEPROM.commit();
  ////          mqtt->sync_pub(String("h"));
  //        }
  //
  //        //            detachInterrupt(0, cb_fn, CHANGE);
  //
  //
  //        //    digitalWrite(RELAY_02_PIN, payload.charAt(i) & RELAY_02_MASK);
  //
  //      }
  //
  //    }
  //  };
  //
  //  attachInterrupt(0, cb_fn, CHANGE);

  //   digitalWrite(4, LOW);
  //   digitalWrite(5, LOW);

  byte val = EEPROM.read(EEPROM_ADDR);
  Serial.println(val, BIN);
  current_state =  val & RELAY_01_MASK;

  Serial.print("START STATE: ");
  Serial.println(current_state);

  Serial.print("START STATE: ");
  Serial.println(current_state);

  Serial.print("START STATE: ");
  Serial.println(current_state);

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
  init_wifi();
  init_mqtt();
}

void loop()
{
  mqtt->loop(wifi);
  prev = millis();
  while (digitalRead(SWITCH_PIN) == LOW) {
    if (millis() - prev > 50) {
      prev = millis();
      Serial.println("OK");
      yield();
      if (digitalRead(RELAY_01_PIN) == LOW) {
        mqtt->sync_pub(String("g"));
      }
      else {
        mqtt->sync_pub(String("h"));
      }

    }
  }
}

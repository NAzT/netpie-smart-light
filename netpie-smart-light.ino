#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif


#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <CMMC_OTA.h>
#include <MqttConnector.h>
#include <WiFiConnector.h>

CMMC_OTA ota;
static MqttConnector *mqtt = NULL;

int state[2];
boolean current_state;
boolean _published_flag = false;
static uint32_t _prev = millis();

#define RELAY_01_PIN 2
#define RELAY_01_MASK 0b0000001

#define EEPROM_ADDR 0x00

#define MQTT_PORT        1883
#define PUBLISH_EVERY    (2*1000) // every 2 seconds


//#define WIFI_SSID       "ESPERT-3020"
//#define WIFI_PASSPHRASE "espertap"

///* WIFI INFO */
#define WIFI_SSID        "@ESPertAP_001"
#define WIFI_PASSPHRASE  "espertap"

WiFiConnector wifi(WIFI_SSID, WIFI_PASSPHRASE);

/* comment ทิ้งถ้าไม่ส่ username/password */
//#define MQTT_USERNAME "GaBH7sxFDUEX0hl"
//#define MQTT_PASSWORD "YHK/C+Q2D4Juz9iPuHCrJL3IJLU="
//#define MQTT_CLIENT_ID "x6Xu0gY0nWyj4inu"

#define MQTT_HOST "mqtt.espert.io"
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define MQTT_CLIENT_ID ""
#define MQTT_PREFIX "/CMMC"

/* SENSOR INFO */
#define DEVICE_NAME "ESP01-SMART-LIGHT"
#define AUTHOR      "Nat Weerawan"
#define BOARD       "ESP-01"
#define SENSOR      "SSR"

#define SWITCH_PIN 0

#include "_receive.h"
#include "_publish.h"
#include "init_mqtt.h"

String queue = "";
//bool wifi_connected = false;
//
//void init_wifi() {
//  wifi.init();
//  wifi.on_connected([&](const void* message)
//  {
//    Serial.print("WIFI CONNECTED WITH IP: ");
//    Serial.println(WiFi.localIP());
//    delay(1000);
//    Serial.println("Connecting MQTT...");
//    mqtt->connect();
//  });
//
//  wifi.on_connecting([&](const void* message)
//  {
//    Serial.print("Connecting to ");
//    Serial.println(wifi.get("ssid") + ", " + wifi.get("password"));
//    delay(200);
//  });
//
//  wifi.connect();
//}

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

//  ota.init();
//  
//  ArduinoOTA.setPort(8266);
//  ArduinoOTA.setHostname("cmmc-smart-light-01");
//
//  // No authentication by default
//  ArduinoOTA.setPassword((const char *)"cmmccmmc");
//  

  Serial.println("HARDWARE INITIALIZED");
}

void setup()
{
  init_hardware();

  WiFi.begin(WIFI_SSID, WIFI_PASSPHRASE);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf ("Connecting to %s:%s\r\n", WIFI_SSID, WIFI_PASSPHRASE);
    delay(300);
  }
  mqtt = init_mqtt();
  mqtt->connect();
}

void loop()
{
  //  wifi.loop();
  mqtt->loop();
//  ota.loop();
  _prev = millis();

  while (digitalRead(SWITCH_PIN) == LOW) {
    mqtt->loop();
    if (millis() - _prev > 120) {
      _prev = millis();
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
      delay(100);
    }
    yield();
  }

}

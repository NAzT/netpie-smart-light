#include <MqttConnector.h>

// MQTT INITIALIZER
MqttConnector* init_mqtt()
{
  MqttConnector *_mqtt = new MqttConnector(MQTT_HOST, MQTT_PORT);
  _mqtt->on_prepare_configuration([&](MqttConnector::Config * config) -> void {
  config->clientId  = String(MQTT_CLIENT_ID);
  config->channelPrefix = String(MQTT_PREFIX);
  config->enableLastWill = true;
  config->retainPublishMessage = false;
  config->firstCapChannel = false;
  // MODE_PUB_ONLY
  // MODE_SUB_ONLY
  // MODE_BOTH
  config->mode = MODE_BOTH;

  config->username = String(MQTT_USERNAME);
  config->password = String(MQTT_PASSWORD);

//  uint8_t mac[6];
//  WiFi.macAddress(mac);
//
//  // device id
//  String macAddr;
//  for (int i = 0; i < 6; ++i)
//  {
//      macAddr += String(mac[i], 16);
//  }

    // FORMAT
    // d:quickstart:<type-id>:<device-id>
//    config->clientId  = String("d:quickstart:esp8266meetup:") + macAddr;
//    config->topicPub  = String("iot-2/evt/status/fmt/json");
  });

  _mqtt->on_after_prepare_configuration([&](MqttConnector::Config config) -> void {
    Serial.printf("[USER] HOST = %s\r\n", config.mqttHost.c_str());
    Serial.printf("[USER] PORT = %lu\r\n", config.mqttPort);
    Serial.printf("[USER] PUB  = %s\r\n", config.topicPub.c_str());
    Serial.printf("[USER] SUB  = %s\r\n", config.topicSub.c_str());
  });

  _mqtt->on_prepare_data(on_prepare_data, PUBLISH_EVERY);
  _mqtt->on_prepare_subscribe([&](MQTT::Subscribe * sub) -> void { });
  _mqtt->on_after_prepare_data([&](JsonObject * root) -> void {
    /**************
    remove prepared data from lib
    root->remove("info");
    JsonObject& data = (*root)["d"];
    data.remove("version");
    data.remove("subscription");
    **************/
  });

  // on_message_arrived located in _receive.h
  _mqtt->on_message(on_message_arrived);

  _mqtt->on_connecting([&](int count, bool * flag) {
    Serial.printf("[%lu] MQTT CONNECTING.. \r\n", count);
    delay(1000);
  });

  _mqtt->on_published([&](const MQTT::Publish & pub) -> void {
    Serial.print("[MQTT] PUBLISHED: ");
    Serial.println(pub.payload_string());
  });

//  _mqtt->on_disconnected([&](void) -> void {
// 
//  }
 
  return _mqtt;
}


WiFiConnector* init_wifi()
{
  // use flash memory ssid & smartconfig
  //wifi = new WiFiConnector();
  wifi = new WiFiConnector(WIFI_SSID, WIFI_PASSPHARSE);

  //  wifi->setLed(16);
  wifi->on_connecting([&](const void* message)
  {
    Serial.print("connecting -> ");
    Serial.println(wifi->counter);
    Serial.println(wifi->get("ssid") + ", " + wifi->get("password"));
    // Serial.println ((char*)message);
    delay(500);
    while (digitalRead(SWITCH_PIN) == LOW) {
      if (millis() - prev > 70) {
        digitalWrite(RELAY_01_PIN, !digitalRead(RELAY_01_PIN));
      }
      yield();
    }
  });

  wifi->on_connected([&](const void* message)
  {
    Serial.println("WIFI CONECTED: ");
    // Print the IP address
    Serial.println(WiFi.localIP());
    // Serial.println ((char*)message);
    mqtt->connect();

  });

  wifi->on_disconnected([&](const void* message)
  {
    Serial.println("WIFI DISCONECTED.");
    // Serial.println ((char*)message);
  });

  wifi->on_smartconfig_waiting([&](const void* message)
  {
    Serial.println("ENTER SMARTCONFIG.");
  });

  wifi->on_smartconfig_done([&](const void* message)
  {
    Serial.println("SMARTCONFIG DONE.");
  });

  wifi->on_smartconfig_processing([&](const void* message)
  {
    // Serial.println("CONFIGURING WIFI..");
    // delay(500);
  });

  return wifi;
}

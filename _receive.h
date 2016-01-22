PubSubClient::callback_t on_message_arrived =
[&](const MQTT::Publish & pub) -> void {
  String topic = pub.topic();
  String payload = pub.payload_string();
  String text = topic + " => " + payload;
  Serial.println(payload);
  Serial.println(payload.toInt());

  if (topic.lastIndexOf("/command") != -1) {



    for (int i = 0; i < payload.length(); i++) {
      current_state = payload.charAt(i) & RELAY_01_MASK;
      Serial.print("GOT PAYLOAD = ");
      Serial.println(payload.charAt(i));
      Serial.print(" => ");
      Serial.println(payload.charAt(i), BIN);
      Serial.print(" MEAN: ");
      Serial.println(current_state);
      Serial.println("=========");
      //    Serial.println(payload.charAt(i) &  0b0000010, BIN);
      //    Serial.println(payload.charAt(i) &  0b0000001, BIN);
      digitalWrite(RELAY_01_PIN, current_state);

      //    digitalWrite(RELAY_02_PIN, payload.charAt(i) & RELAY_02_MASK);
      EEPROM.write(EEPROM_ADDR, payload.charAt(i));
      EEPROM.commit();
    }
  }
  //  if (payload.length() == 1) {
  //    char buf;
  //    payload.getBytes(&buf, 1);
  //    Serial.println((char)buf);
  //    Serial.println(buf, BIN);
  //    Serial.println(buf, DEC);
  //    Serial.println(buf, DEC);
  //  }

};

#include "MqttModule.h"

extern int temp;
extern CMMC_SENSOR_DATA_T data1;

#define MQTT_CONFIG_FILE "/mymqtt.json"
void MqttModule::config(CMMC_System *os, AsyncWebServer *server)
{
  strcpy(this->path, "/api/mqtt");
  this->_serverPtr = server;
  this->_managerPtr = new CMMC_ConfigManager(MQTT_CONFIG_FILE);
  this->_managerPtr->init();
  this->_managerPtr->load_config([&](JsonObject *root, const char *content) {
    if (root == NULL)
    {
      Serial.print("mqtt.json failed. >");
      Serial.println(content);
      return;
    }
    Serial.println(content);
    Serial.println("[user] mqtt config json loaded..");
    char mqtt_host[40] = "";
    char mqtt_user[40] = "";
    char mqtt_pass[40] = "";
    char mqtt_clientId[40] = "";
    char mqtt_prefix[40] = "";
    char mqtt_port[10] = "";
    char mqtt_device_name[20] = "";
    bool lwt;
    uint32_t pubEveryS;
    const char *mqtt_configs[] = {(*root)["host"],
                                  (*root)["username"], (*root)["password"],
                                  (*root)["clientId"], (*root)["port"],
                                  (*root)["deviceName"],
                                  (*root)["prefix"], // [6]
                                  (*root)["lwt"],
                                  (*root)["publishRateSecond"]};

    if (mqtt_configs[0] != NULL)
    {
      strcpy(mqtt_host, mqtt_configs[0]);
      strcpy(mqtt_user, mqtt_configs[1]);
      strcpy(mqtt_pass, mqtt_configs[2]);
      strcpy(mqtt_clientId, mqtt_configs[3]);
      strcpy(mqtt_port, mqtt_configs[4]);
      strcpy(mqtt_device_name, mqtt_configs[5]);
      strcpy(mqtt_prefix, mqtt_configs[6]);

      lwt = String(mqtt_configs[7]).toInt();
      pubEveryS = String(mqtt_configs[8]).toInt();

      if (strcmp(mqtt_device_name, "") == 0)
      {
        sprintf(mqtt_device_name, "%08x", ESP.getChipId());
      }
      else
      {
        Serial.printf("DEVICE NAME = %s\r\n", mqtt_device_name);
      }

      if (strcmp(mqtt_clientId, "") == 0)
      {
        sprintf(mqtt_clientId, "%08x", ESP.getChipId());
      }
    }
    MQTT_HOST = String(mqtt_host);
    MQTT_USERNAME = String(mqtt_user);
    MQTT_PASSWORD = String(mqtt_pass);
    MQTT_CLIENT_ID = String(mqtt_clientId);
    MQTT_PORT = String(mqtt_port).toInt();
    MQTT_PREFIX = String(mqtt_prefix);
    PUBLISH_EVERY = pubEveryS * 1000L;
    MQTT_LWT = lwt;
    DEVICE_NAME = String(mqtt_device_name);
  });

  this->configWebServer();
};

void MqttModule::configWebServer()
{
  static MqttModule *that = this;
  _serverPtr->on(this->path, HTTP_POST, [&](AsyncWebServerRequest *request) {
    String output = that->saveConfig(request, this->_managerPtr);
    request->send(200, "application/json", output);
  });
}

void MqttModule::configLoop() {
    if(u8g2 == NULL) {
    u8g2 = new U8G2_ST7920_128X64_1_SW_SPI(U8G2_R0, /* clock=*/14, /* data=*/13, /* CS=*/12);
    u8g2->begin();

    String strID = String(ESP.getChipId(), HEX);

    u8g2->firstPage();
    do
    {
      u8g2->setFont(u8g2_font_ncenB10_tr);
      u8g2->setCursor(0, 11);
      u8g2->print("3E-BOT");
      u8g2->setFont(u8g2_font_ncenB08_tr);
      u8g2->setCursor(0, 25);
      u8g2->print(" CMMC-");
      u8g2->print(strID);
    } while (u8g2->nextPage());
  }
}

void MqttModule::setup()
{
  Serial.println("MqttModule::setup");
  init_mqtt();

  if(u8g2 == NULL) {
    u8g2 = new U8G2_ST7920_128X64_1_SW_SPI(U8G2_R0, /* clock=*/14, /* data=*/13, /* CS=*/12);
    u8g2->begin();
  }


  Wire.begin(4, 5);
  bme = new Adafruit_BME280();
  bool bmeStatus;
  bmeStatus = bme->begin(0x76);
  if (!bmeStatus)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    u8g2->firstPage();
    do
    {
      u8g2->setFont(u8g2_font_ncenB10_tr);
      u8g2->setCursor(0, 11);
      u8g2->print("3E-BOT");
      u8g2->setFont(u8g2_font_ncenB08_tr);
      u8g2->setCursor(0, 25);
      u8g2->print(" Could not find BME");
      u8g2->setCursor(0, 35);
      u8g2->print("   check wiring !");
    } while (u8g2->nextPage());
    delay(3000);
  }

  // rtc = new RTC_DS3231();
  // rtc->begin();
  // rtc->adjust(DateTime(F(__DATE__), F(__TIME__))); // uncomment for adjust DateTime
};

void MqttModule::loop()
{
  DateTime now = rtc->now();

  // Serial.print(now.year(), DEC);
  // Serial.print('/');
  // Serial.print(now.month(), DEC);
  // Serial.print('/');
  // Serial.print(now.day(), DEC);
  // Serial.print(" (");
  // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  // Serial.print(") ");
  // Serial.print(now.hour(), DEC);
  // Serial.print(':');
  // Serial.print(now.minute(), DEC);
  // Serial.print(':');
  // Serial.print(now.second(), DEC);
  // Serial.println();

  data1.field1 = bme->readTemperature();
  data1.field2 = bme->readHumidity();
  data1.field3 = bme->readPressure() / 100.0;
  data1.field4 = bme->readAltitude(1013.25);

  if (data1.field1 <= 0 || data1.field1 >= 100)
  {
    u8g2->firstPage();
    do
    {
      u8g2->setFont(u8g2_font_ncenB10_tr);
      u8g2->setCursor(0, 11);
      u8g2->print("3E-BOT");
      u8g2->setFont(u8g2_font_ncenB08_tr);
      u8g2->setCursor(0, 25);
      u8g2->print(" Sensor ERROR");
      u8g2->setCursor(0, 35);
      u8g2->print("   check wiring !");
    } while (u8g2->nextPage());
  }
  else
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000)
    {
      previousMillis = currentMillis;

      u8g2->firstPage();
      do
      {
        u8g2->setFont(u8g2_font_ncenB10_tr);
        //u8g2->drawFrame(0, 0, 128, 31);
        //u8g2->drawFrame(0, 33, 128, 31);

        dtostrf(bme->readTemperature(), 5, 1, _tempString);
        dtostrf(bme->readHumidity(), 5, 1, _humidString);

        u8g2->drawStr(15, 13, "Temperature");
        u8g2->drawStr(33, 28, _tempString);
        u8g2->drawStr(70, 28, "*C");

        u8g2->drawStr(26, 46, "Humidity");
        u8g2->drawStr(40, 60, _humidString);
        u8g2->drawStr(75, 60, "%");

        //u8g2->setFont(u8g2_font_ncenB10_tr);
        //u8g2->drawStr(0, 11, "3E-BOT");
        //u8g2->setFont(u8g2_font_ncenB08_tr);
        //u8g2->setCursor(0, 20);
        //u8g2->print(" Temp = " + String(bme->readTemperature()));
        //u8g2->setCursor(0, 30);
        //u8g2->print(" Humid = " + String(bme->readHumidity()));
        //u8g2->setCursor(0, 40);
        //u8g2->print(" Pressure = " + String(bme->readPressure() / 100.0));
        //u8g2->setCursor(0, 50);
        //u8g2->print(" Alt = " + String(bme->readAltitude(1013.25)));
        // u8g2->setCursor(0, 60);
        // u8g2->print(" Time = " + String(now.hour()) + ":" + String(now.minute()));
      } while (u8g2->nextPage());
    }
  }
  mqtt->loop();
};

// MQTT INITIALIZER

MqttConnector *MqttModule::init_mqtt()
{
  this->mqtt = new MqttConnector(this->MQTT_HOST.c_str(), this->MQTT_PORT);

  mqtt->on_connecting([&](int counter, bool *flag) {
    Serial.printf("[%lu] MQTT CONNECTING.. \r\n", counter);
    if (counter >= MQTT_CONNECT_TIMEOUT)
    {
      ESP.reset();
    }
    delay(1000);
  });

  mqtt->on_prepare_configuration([&](MqttConnector::Config *config) -> void {
    Serial.printf("lwt = %lu\r\n", MQTT_LWT);
    config->clientId = MQTT_CLIENT_ID;
    config->channelPrefix = MQTT_PREFIX;
    config->enableLastWill = MQTT_LWT;
    config->retainPublishMessage = false;
    /*
        config->mode
        ===================
        | MODE_BOTH       |
        | MODE_PUB_ONLY   |
        | MODE_SUB_ONLY   |
        ===================
    */
    config->mode = MODE_BOTH;
    config->firstCapChannel = false;

    config->username = String(MQTT_USERNAME);
    config->password = String(MQTT_PASSWORD);

    // FORMAT
    // d:quickstart:<type-id>:<device-id>
    //config->clientId  = String("d:quickstart:esp8266meetup:") + macAddr;
    config->topicPub = MQTT_PREFIX + String(DEVICE_NAME) + String("/status");
  });

  mqtt->on_after_prepare_configuration([&](MqttConnector::Config config) -> void {
    String humanTopic = MQTT_PREFIX + DEVICE_NAME + String("/$/+");
    Serial.printf("[USER] HOST = %s\r\n", config.mqttHost.c_str());
    Serial.printf("[USER] PORT = %d\r\n", config.mqttPort);
    Serial.printf("[USER] PUB  = %s\r\n", config.topicPub.c_str());
    Serial.printf("[USER] SUB  = %s\r\n", config.topicSub.c_str());
    Serial.printf("[USER] SUB  = %s\r\n", humanTopic.c_str());
    // sub->add_topic(MQTT_PREFIX + String("/") + String(myName) + String("#"));
    // sub->add_topic(MQTT_PREFIX + "/" + MQTT_CLIENT_ID + "/$/+");
  });

  if (mqtt == NULL)
  {
    Serial.println("MQTT is undefined.");
  }

  register_publish_hooks(mqtt);
  register_receive_hooks(mqtt);

  Serial.println("connecting to mqtt..");
  mqtt->connect();
  return mqtt;
}

void MqttModule::register_receive_hooks(MqttConnector *mqtt)
{
  mqtt->on_subscribe([&](MQTT::Subscribe *sub) -> void {
    Serial.printf("onSubScribe myName = %s \r\n", DEVICE_NAME.c_str());
    sub->add_topic(MQTT_PREFIX + DEVICE_NAME + String("/$/+"));
    sub->add_topic(MQTT_PREFIX + MQTT_CLIENT_ID + String("/$/+"));
    sub->add_topic(MQTT_PREFIX + DEVICE_NAME + String("/status"));
    Serial.println("done on_subscribe...");
    Serial.printf("publish every %lu s\r\n", PUBLISH_EVERY);
  });

  mqtt->on_before_message_arrived_once([&](void) {});

  mqtt->on_message([&](const MQTT::Publish &pub) {});

  mqtt->on_after_message_arrived([&](String topic, String cmd, String payload) {
    // Serial.printf("recv topic: %s\r\n", topic.c_str());
    // Serial.printf("recv cmd: %s\r\n", cmd.c_str());
    // Serial.printf("payload: %s\r\n", payload.c_str());
    if (cmd == "$/command")
    {
      if (payload == "ON")
      {
        Serial.println("ON");
        // gpio.on();
        // relayPinState = 1;
      }
      else if (payload == "OFF")
      {
        // relayPinState = 0;
        // gpio.off();
        Serial.println("OFF");
      }
      else if (payload == "FORCE_CONFIG")
      {
        SPIFFS.remove("/enabled");
        ESP.restart();
      }
    }
    else if (cmd == "$/reboot")
    {
      ESP.restart();
    }
    else if (cmd == "status")
    {
      // Serial.println("sent & recv.");
    }
    else
    {
      Serial.println("Another message arrived.");
      // another message.
    }
    // lastRecv = millis();
  });
}

void MqttModule::register_publish_hooks(MqttConnector *mqtt)
{
  mqtt->on_prepare_data_once([&](void) {
    Serial.println("initializing sensor...");
  });

  mqtt->on_before_prepare_data([&](void) {
  });

  mqtt->on_prepare_data([&](JsonObject *root) {
    JsonObject &data = (*root)["d"];
    JsonObject &info = (*root)["info"];
    // data["appVersion"] = LEGEND_APP_VERSION;
    data["myName"] = DEVICE_NAME;
    data["millis"] = millis();

    if (data1.field1 >= 0 && data1.field1 <= 100)
    {
      data["temp"] = data1.field1;
    }

    if (data1.field2 >= 0 && data1.field2 <= 100)
    {
      data["humid"] = data1.field2;
    }

    if (data1.field3 >= 0 && data1.field3 <= 10000)
    {
      data["pressure"] = data1.field3;
    }

    if (data1.field4 >= 0 && data1.field4 <= 10000)
    {
      data["altitude"] = data1.field4;
    }
    data["updateInterval"] = PUBLISH_EVERY;

    // Serial.printf("field1 = %lu \r\n", sensorData.field1);
    // Serial.printf("field2 = %lu \r\n", sensorData.field2);
    // Serial.printf("field3 = %lu \r\n", sensorData.field3);
    // Serial.printf("field4 = %lu \r\n", sensorData.field4);
    // Serial.printf("field5 = %lu \r\n", sensorData.field5);
    // Serial.printf("field6 = %lu \r\n", sensorData.field6);

    // if (sensorData.field1) { data["field1"] = sensorData.field1; }
    // if (sensorData.field2) { data["field2"] = sensorData.field2; }
    // if (sensorData.field3) { data["field3"] = sensorData.field3; }
    // if (sensorData.field4) { data["field4"] = sensorData.fi    // Serial.printf("field1 = %lu \r\n", sensorData.field1);
    // Serial.printf("field2 = %lu \r\n", sensorData.field2);
    // Serial.printf("field3 = %lu \r\n", sensorData.field3);
    // Serial.printf("field4 = %lu \r\n", sensorData.field4);
    // Serial.printf("field5 = %lu \r\n", sensorData.field5);
    // Serial.printf("field6 = %lu \r\n", sensorData.field6);

    // if (sensorData.field1) { data["field1"] = sensorData.field1; }
    // if (sensorData.field2) { data["field2"] = sensorData.field2; }
    // if (sensorData.field3) { data["field3"] = sensorData.field3; }
    // if (sensorData.field4) { data["field4"] = sensorData.field4; }
    // if (sensorData.field5) { data["field5"] = sensorData.field5; }
    // if (sensorData.field6) { data["field6"] = sensorData.field6; }
    // if (sensorData.field7) { data["field7"] = sensorData.field7; }
    // if (sensorData.field8) { data["field8"] = sensorData.field8; }
    // if (sensorData.ms) { data["ms"] = sensorData.ms; }
    // if (sensorData.battery) { data["battery"] = sensorData.battery; }eld4; }
    // if (sensorData.field5) { data["field5"] = sensorData.fi    // Serial.printf("field1 = %lu \r\n", sensorData.field1);
    // Serial.printf("field2 = %lu \r\n", sensorData.field2);
    // Serial.printf("field3 = %lu \r\n", sensorData.field3);
    // Serial.printf("field4 = %lu \r\n", sensorData.field4);
    // Serial.printf("field5 = %lu \r\n", sensorData.field5);
    // Serial.printf("field6 = %lu \r\n", sensorData.field6);

    // if (sensorData.field1) { data["field1"] = sensorData.field1; }
    // if (sensorData.field2) { data["field2"] = sensorData.field2; }
    // if (sensorData.field3) { data["field3"] = sensorData.field3; }
    // if (sensorData.field4) { data["field4"] = sensorData.field4; }
    // if (sensorData.field5) { data["field5"] = sensorData.field5; }
    // if (sensorData.field6) { data["field6"] = sensorData.field6; }
    // if (sensorData.field7) { data["field7"] = sensorData.field7; }
    // if (sensorData.field8) { data["field8"] = sensorData.field8; }
    // if (sensorData.ms) { data["ms"] = sensorData.ms; }
    // if (sensorData.battery) { data["battery"] = sensorData.battery; }eld5; }
    // if (sensorData.field6) { data["field6"] = sensorData.fi    // Serial.printf("field1 = %lu \r\n", sensorData.field1);
    // Serial.printf("field2 = %lu \r\n", sensorData.field2);
    // Serial.printf("field3 = %lu \r\n", sensorData.field3);
    // Serial.printf("field4 = %lu \r\n", sensorData.field4);
    // Serial.printf("field5 = %lu \r\n", sensorData.field5);
    // Serial.printf("field6 = %lu \r\n", sensorData.field6);

    // if (sensorData.field1) { data["field1"] = sensorData.field1; }
    // if (sensorData.field2) { data["field2"] = sensorData.field2; }
    // if (sensorData.field3) { data["field3"] = sensorData.field3; }
    // if (sensorData.field4) { data["field4"] = sensorData.field4; }
    // if (sensorData.field5) { data["field5"] = sensorData.field5; }
    // if (sensorData.field6) { data["field6"] = sensorData.field6; }
    // if (sensorData.field7) { data["field7"] = sensorData.field7; }
    // if (sensorData.field8) { data["field8"] = sensorData.field8; }
    // if (sensorData.ms) { data["ms"] = sensorData.ms; }
    // if (sensorData.battery) { data["battery"] = sensorData.battery; }eld6; }
    // if (sensorData.field7) { data["field7"] = sensorData.fi    // Serial.printf("field1 = %lu \r\n", sensorData.field1);
    // Serial.printf("field2 = %lu \r\n", sensorData.field2);
    // Serial.printf("field3 = %lu \r\n", sensorData.field3);
    // Serial.printf("field4 = %lu \r\n", sensorData.field4);
    // Serial.printf("field5 = %lu \r\n", sensorData.field5);
    // Serial.printf("field6 = %lu \r\n", sensorData.field6);

    // if (sensorData.field1) { data["field1"] = sensorData.field1; }
    // if (sensorData.field2) { data["field2"] = sensorData.field2; }
    // if (sensorData.field3) { data["field3"] = sensorData.field3; }
    // if (sensorData.field4) { data["field4"] = sensorData.field4; }
    // if (sensorData.field5) { data["field5"] = sensorData.field5; }
    // if (sensorData.field6) { data["field6"] = sensorData.field6; }
    // if (sensorData.field7) { data["field7"] = sensorData.field7; }
    // if (sensorData.field8) { data["field8"] = sensorData.field8; }
    // if (sensorData.ms) { data["ms"] = sensorData.ms; }
    // if (sensorData.battery) { data["battery"] = sensorData.battery; }eld7; }
    // if (sensorData.field8) { data["field8"] = sensorData.field8; }
    // if (sensorData.ms) { data["ms"] = sensorData.ms; }
    // if (sensorData.battery) { data["battery"] = sensorData.battery; }
    Serial.println("PUBLISHING...!");
    // Serial.printf("temp = %d\r\n", bme->readTemperature());
  },
                        PUBLISH_EVERY);

  mqtt->on_after_prepare_data([&](JsonObject *root) {
    /**************
      JsonObject& data = (*root)["d"];
      data.remove("version");
      data.remove("subscription");
    **************/
  });
}
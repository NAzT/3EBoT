
#ifndef CMMC_MQTT_MODULE_H
#define CMMC_MQTT_MODULE_H

#include <CMMC_Module.h>
#include <MqttConnector.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "RTClib.h"
#include <U8g2lib.h>
#include <SPI.h>
#include <CMMC_Sensor.h>

class MqttModule : public CMMC_Module
{
public:
  void config(CMMC_System *os, AsyncWebServer *server);
  void configLoop();
  void setup();
  void loop();
  
protected:
  void configWebServer();
  MqttConnector *init_mqtt();
  void register_receive_hooks(MqttConnector *mqtt);
  void register_publish_hooks(MqttConnector *mqtt);

private:
  MqttConnector *mqtt;
  String MQTT_HOST;
  String MQTT_USERNAME;
  String MQTT_PASSWORD;
  String MQTT_CLIENT_ID;
  String MQTT_PREFIX;
  String DEVICE_NAME;
  int MQTT_PORT;
  int PUBLISH_EVERY;
  int MQTT_CONNECT_TIMEOUT;
  bool MQTT_LWT;

  Adafruit_BME280 *bme;
  U8G2_ST7920_128X64_1_SW_SPI *u8g2 = NULL;
  char _tempString[10];
  char _humidString[10];
  unsigned long previousMillis = 0;
  

  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  RTC_DS3231 *rtc;

  CMMC_SENSOR_DATA_T data1;
};

#endif
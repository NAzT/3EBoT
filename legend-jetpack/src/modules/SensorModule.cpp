#include "SensorModule.h"
#include "LCDModule.h"

extern LCDModule* lcdModule; 

void SensorModule::configLoop() {
}

void SensorModule::config(CMMC_System *os, AsyncWebServer *server)
{
  if (lcdModule) {
    // lcdModule->displayConfigWiFi(); 
  }
}

void SensorModule::configWebServer()
{
  static SensorModule *that = this;
}

void SensorModule::setup()
{
  Wire.begin(4, 5);
  bme = new Adafruit_BME280();
  bool bmeStatus;
  bmeStatus = bme->begin(0x76);
  if (!bmeStatus) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
}

void SensorModule::loop() 
{ 
  interval.every_ms(5000, [&]() { 
    _temperature = bme->readTemperature();
    _humidity = bme->readHumidity();
    
    Serial.println("read sensor...");
    data1.field1 = bme->readTemperature();
    data1.field2 = bme->readHumidity();
    data1.field3 = bme->readPressure() / 100.0;
    data1.field4 = bme->readAltitude(1013.25); 
  });
}

String SensorModule::getTemperatureString() { 
  char buffer[10];
  sprintf(buffer,"%.1f", _temperature);
  return String(buffer); 
}

String SensorModule::getHumidityString() { 
  return String((int)_humidity); 
}
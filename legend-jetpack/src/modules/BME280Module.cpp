#include "BME280Module.h"
extern int temp;

void BME280Module::config(CMMC_System *os, AsyncWebServer *server)
{
  static BME280Module *that = this;
}

void BME280Module::configLoop() {}

void BME280Module::setup()
{
  Wire.begin(4, 5);
  bme = new Adafruit_BMP280();
  bool status;
  status = bme->begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
  Serial.println(F("BMP280 test"));
}

void BME280Module::loop()
{
  Serial.print(F("Temperature = "));
  Serial.print(bme->readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bme->readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bme->readAltitude(1013.25)); // this should be adjusted to your local forcase
  Serial.println(" m");

  Serial.println();
  delay(2000);
}

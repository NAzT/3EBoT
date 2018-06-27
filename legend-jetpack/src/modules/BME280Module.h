#include <CMMC_Module.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


#ifndef CMMC_BME280_MODULE_H
#define CMMC_BME280_MODULE_H 

class BME280Module: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void configLoop();
  protected:
  private:
    Adafruit_BMP280 *bme;
};

#endif
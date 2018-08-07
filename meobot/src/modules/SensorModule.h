#include <CMMC_Module.h>

#ifndef CMMC_SensorModule_MODULE_H
#define CMMC_SensorModule_MODULE_H 

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <CMMC_Sensor.h>
#include <CMMC_Interval.h>
#include <Adafruit_BME280.h>

const int MAX_ARRAY = 10;
class SensorModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void isLongPressed(); 
    void configLoop();
    String getTemperatureString();
    String getHumidityString();
    String getTemperature();
    String getHumidity();

  protected:
    void configWebServer();
  private:
    Adafruit_BME280 *bme;
    CMMC_SENSOR_DATA_T data1; 
    CMMC_Interval interval;
    float temp_array[MAX_ARRAY] = { 0.0 };
    float humid_array[MAX_ARRAY] = { 0.0 };
    float pressure_array[MAX_ARRAY] = { 0.0 };

    float _temperature;
    float _humidity;
    float _pressure;

    unsigned long counter = 0;
};

#endif
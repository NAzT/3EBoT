#include <CMMC_Module.h>

#ifndef CMMC_ADCSensors_MODULE_H
#define CMMC_ADCSensors_MODULE_H 

#include <Wire.h>
#include <CMMC_Sensor.h>
#include <CMMC_Interval.h> 
#include <Adafruit_ADS1015.h> 
#include "../utils/utils.h"


const int MAX_ARRAY = 3;
class ADCSensors: public CMMC_Module {
  public:
    float soil_moisture_percent = 0.0;
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void isLongPressed(); 
    void configLoop();
    float getAnalog(int slot);
    int soil_enable = true; 
    uint8_t _pageIdx = 0;
    uint8_t MAX_PAGE = 3;
    bool _pin0StateDirty = false;
  protected:
    void configWebServer();
  private:
    CMMC_SENSOR_DATA_T data1; 
    CMMC_Interval interval;
    
    float temp_array[2][MAX_ARRAY] = { {0.0}, {0.0} };
    float humid_array[2][MAX_ARRAY] = { {0.0}, {0.0} };
    float pressure_array[2][MAX_ARRAY] = { {0.0}, {0.0} }; 
    float adc0_array[2][MAX_ARRAY] = { {0.0}, {0.0} };

    float _temperature[2];
    float _humidity[2];
    float _pressure[2]; 
    float _adc0[2];
    int soil_max;
    int soil_min;
    int soil_moisture;

    unsigned long counter = 0;
    Adafruit_ADS1115 *ads; 
};

#endif
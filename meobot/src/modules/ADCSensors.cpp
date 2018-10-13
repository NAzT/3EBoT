#include "ADCSensors.h"
#include "LCDModule.h"

extern LCDModule *lcdModule;

void ADCSensors::configLoop()
{
}

void ADCSensors::config(CMMC_System *os, AsyncWebServer *server)
{
  strcpy(this->path, "/api/sensors");
  this->_serverPtr = server;
  this->_managerPtr = new CMMC_ConfigManager("/sensors.json");
  this->_managerPtr->init();
  this->_managerPtr->load_config([&](JsonObject * root, const char *content) {
    if (root == NULL)
    {
      Serial.print("sensors.json failed. >");
      Serial.println(content);
      return;
    }
    else {
      Serial.println(content); 
      const char *soil_configs [] = {(*root)["soil_enable"],
                              (*root)["soil_max"], (*root)["soil_min"],
                              (*root)["soil_moisture"]
                              };
      soil_enable = String(soil_configs[0]).toInt();
      soil_max  = String(soil_configs[1]).toInt();
      soil_min = String(soil_configs[2]).toInt();
      soil_moisture = String(soil_configs[3]).toInt();
    }
  });
  this->configWebServer();
  // if (lcdModule)
  // {
  //   // lcdModule->displayConfigWiFi();
  // }
}

void ADCSensors::configWebServer()
{
  _serverPtr->on(this->path, HTTP_POST, [&](AsyncWebServerRequest *request) {
    String output = saveConfig(request, this->_managerPtr);
    request->send(200, "application/json", output);
  });
}


float ADCSensors::getAnalog(int slot)
{
  return _adc0[slot-1];
}

void ADCSensors::setup()
{ 
  Wire.begin(4, 5);
  pinMode(0, INPUT_PULLUP);

  ads = new Adafruit_ADS1115(0x48); 
  ads->setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV 
  ads->setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads->begin(); 
}

void ADCSensors::loop()
{
  int pin0State = digitalRead(0);
  if (!_pin0StateDirty && pin0State == LOW) {
    this->_pin0StateDirty = true; 
    _pageIdx  = ++_pageIdx % this->MAX_PAGE;
    Serial.println(_pageIdx);
  }

  interval.every_ms(2000, [&]() {
    int idx = counter % MAX_ARRAY;

    if (soil_enable) {
      int16_t adc0;
      adc0 = ads->readADC_SingleEnded(0);
      adc0_array[0][idx] = adc0; 
      Serial.printf("[ENABLE] soil sensor is enabled!\r\n");
    }
    else {
      Serial.printf("[DISABLE] soil sensor is not enabled!\r\n");
    }

    if (counter < MAX_ARRAY)
    { 
      
      for(size_t i = 0; i < 2; i++)
      {
        _temperature[i] = CMMC_Utils::median(temp_array[i], idx + 1);
        _humidity[i] = CMMC_Utils::median(humid_array[i], idx + 1);
        _pressure[i] = CMMC_Utils::median(pressure_array[i], idx + 1);
        _adc0[i] = CMMC_Utils::median(adc0_array[i], idx + 1);
      }
      
    }
    else
    {
      for(size_t i = 0; i < 2; i++)
      {
        _temperature[i] = CMMC_Utils::median(temp_array[i], MAX_ARRAY);
        _humidity[i] = CMMC_Utils::median(humid_array[i], MAX_ARRAY);
        _pressure[i] = CMMC_Utils::median(pressure_array[i], MAX_ARRAY);
        _adc0[i] = CMMC_Utils::median(adc0_array[i], MAX_ARRAY);
      }
      
    }
    
    float a0_percent = map(_adc0[0], soil_min, soil_max, 100, 0);
    if (soil_enable) {
      soil_moisture_percent = a0_percent; 
      if (a0_percent <= soil_moisture) {
        digitalWrite(2, HIGH); 
      } 
      else {
        digitalWrite(2, LOW); 
      }
    }

    Serial.printf("temp[0]=%.2f humid[0]=%.2f, adc0[0]=%.2f, %f%%\r\n", 
      this->_temperature[0], this->_humidity[0], this->_adc0[0], soil_moisture_percent); 
    Serial.printf("temp[1]=%.2f humid[1]=%.2f, adc0[1]=%.2f, %f%%\r\n", 
      this->_temperature[1], this->_humidity[1], this->_adc0[1], soil_moisture_percent); 

    counter++;
  });
}
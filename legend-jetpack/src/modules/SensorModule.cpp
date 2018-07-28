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
}

void SensorModule::loop() 
{ 
}

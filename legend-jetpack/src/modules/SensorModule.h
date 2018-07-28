#include <CMMC_Module.h>

#ifndef CMMC_SensorModule_MODULE_H
#define CMMC_SensorModule_MODULE_H 

class SensorModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void isLongPressed(); 
    void configLoop();
  protected:
    void configWebServer();
  private:
};

#endif
#include <CMMC_Module.h>
#include <U8g2lib.h>
#include <SPI.h>

#ifndef CMMC_WIFI_MODULE_H
#define CMMC_WIFI_MODULE_H 

class WiFiModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void isLongPressed();
  protected:
    void configWebServer();
  private:
    char sta_ssid[30] = "";
    char sta_pwd[30] = "";
    char ap_pwd[30] = ""; 
    void _init_sta(); 

    U8G2_ST7920_128X64_1_SW_SPI *u8g2;
};

#endif
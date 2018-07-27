#include <CMMC_Module.h>
#include <U8g2lib.h>
#include <SPI.h>

#ifndef CMMC_LCD_MODULE_H
#define CMMC_LCD_MODULE_H 

class LCDModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void isLongPressed(); 
    void configLoop();
    void displayConfigWiFi();
  protected:
    void configWebServer();
  private:
    U8G2_ST7920_128X64_1_SW_SPI *u8g2;
    void displayLogo();
};

#endif
#include <CMMC_Module.h>
#include <U8g2lib.h>
#include <SPI.h>

#ifndef CMMC_LCD128x64_MODULE_H
#define CMMC_LCD128x64_MODULE_H 

class LCD128x64Module: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void configLoop();
    void fristPrint();
  protected:
  private:
  
    U8G2_ST7920_128X64_1_SW_SPI *u8g2;
};

#endif
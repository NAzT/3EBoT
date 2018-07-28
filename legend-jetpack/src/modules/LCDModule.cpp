#include "LCDModule.h"
#include "NTPModule.h"
#include "logo.h"

extern NTPModule* ntpModule;

void LCDModule::displayLogo() {
    u8g2->firstPage();
  do
  {
    u8g2->drawXBM(0, 0, 128, 64, logo);
  } while (u8g2->nextPage()); 
  delay(500);
}

void LCDModule::displayConfigWiFi() {
  char strID[10];
  sprintf(strID, "%08x", ESP.getChipId());

  u8g2->firstPage();
  do {
      u8g2->setFont(u8g2_font_ncenB10_tr);
      u8g2->setCursor(0, 11);
      u8g2->print("3E-BOT");
      u8g2->setFont(u8g2_font_ncenB08_tr);
      u8g2->setCursor(0, 25);
      u8g2->print(" CMMC-");
      u8g2->print(strID);
  } while (u8g2->nextPage()); 
}

void LCDModule::displayConnectingWiFi(const char* sta_ssid) {
    u8g2->firstPage();
    do
    {
      u8g2->setFont(u8g2_font_ncenB10_tr);
      u8g2->setCursor(0, 11);
      u8g2->print("3E-BOT");
      u8g2->setFont(u8g2_font_ncenB08_tr);
      u8g2->setCursor(0, 25);
      u8g2->print(" Connecting to ");
      u8g2->print(sta_ssid);
    } while (u8g2->nextPage()); 
}

void LCDModule::displayWiFiConnected() {
    u8g2->firstPage();
    do
    {
      u8g2->setFont(u8g2_font_ncenB10_tr);
      u8g2->setCursor(0, 11);
      u8g2->print("3E-BOT");
      u8g2->setFont(u8g2_font_ncenB08_tr);
      u8g2->setCursor(0, 25);
      u8g2->print("WiFi Connected.");
    } while (u8g2->nextPage()); 
}

void LCDModule::configLoop() {
  if (digitalRead(0) == HIGH) {
    while(digitalRead(0) == HIGH) {
      delay(10); 
    } 
    File f = SPIFFS.open("/enabled", "a+");
    delay(100);
    ESP.restart();
  }
}

void LCDModule::config(CMMC_System *os, AsyncWebServer *server)
{
  u8g2 = new U8G2_ST7920_128X64_1_SW_SPI(U8G2_R0, /* clock=*/14, /* data=*/13, /* CS=*/12);
  u8g2->begin(); 
  displayLogo();
}

void LCDModule::configWebServer()
{
  static LCDModule *that = this;
}

void LCDModule::setup()
{
}

void LCDModule::loop() {
  interval.every_ms(1000, [&]() {
    u8g2->firstPage();
    do
    {
      u8g2->setFont(u8g2_font_ncenB10_tr);
      u8g2->setCursor(0, 11);
      u8g2->print("3E-BOT");
      u8g2->setFont(u8g2_font_ncenB08_tr);
      u8g2->setCursor(0, 25);
      u8g2->print(ntpModule->getTimeString());
    } while (u8g2->nextPage()); 
  }); 
} 
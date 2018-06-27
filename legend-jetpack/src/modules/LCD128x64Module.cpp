#include "LCD128x64Module.h"

void LCD128x64Module::config(CMMC_System *os, AsyncWebServer *server)
{
  static LCD128x64Module *that = this;
  u8g2 = new U8G2_ST7920_128X64_1_SW_SPI(U8G2_R0, /* clock=*/14, /* data=*/13, /* CS=*/12);
}

void LCD128x64Module::configLoop() {}

void LCD128x64Module::fristPrint(void)
{
  u8g2->setFont(u8g2_font_ncenB10_tr);
  u8g2->drawStr(0, 11, String("Hello my First Display").c_str());
  delay(3000);
}

void LCD128x64Module::setup()
{
  u8g2->begin();
  fristPrint();
}

void LCD128x64Module::loop()
{
  if (millis() % 1000 == 0)
  {
    u8g2->firstPage();
    do
    {
      u8g2->setFont(u8g2_font_ncenB10_tr);
      u8g2->drawStr(0, 11, "Weather Station");
      u8g2->setFont(u8g2_font_unifont_t_symbols);
      //    u8g2->drawGlyph(5, 30, 0x2604);
      u8g2->drawUTF8(5, 30, "Snowman ☃");
      u8g2->setCursor(5, 50);
      // u8g2->print("count = " + String(count));
    } while (u8g2->nextPage());
  }
}

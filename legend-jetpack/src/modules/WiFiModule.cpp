#include "WiFiModule.h"

#define WIFI_CONFIG_FILE "/wifi.json"

void WiFiModule::config(CMMC_System *os, AsyncWebServer *server)
{
  strcpy(this->path, "/api/wifi/sta");
  static WiFiModule *that = this;
  this->_serverPtr = server;
  this->_managerPtr = new CMMC_ConfigManager(WIFI_CONFIG_FILE);
  this->_managerPtr->init();
  this->_managerPtr->load_config([](JsonObject *root, const char *content) {
    if (root == NULL)
    {
      Serial.print("wifi.json failed. >");
      Serial.println(content);
      return;
    }
    Serial.println("[user] wifi config json loaded..");
    const char *sta_config[2];
    sta_config[0] = (*root)["sta_ssid"];
    sta_config[1] = (*root)["sta_password"];
    if ((sta_config[0] == NULL) || (sta_config[1] == NULL))
    {
      Serial.println("NULL..");
      SPIFFS.remove("/enabled");
      return;
    };
    strcpy(that->sta_ssid, sta_config[0]);
    strcpy(that->sta_pwd, sta_config[1]);
  });
  this->configWebServer();
}

void WiFiModule::configWebServer()
{
  static WiFiModule *that = this;
  _serverPtr->on(this->path, HTTP_POST, [&](AsyncWebServerRequest *request) {
    String output = that->saveConfig(request, this->_managerPtr);
    request->send(200, "application/json", output);
  });
}

void WiFiModule::isLongPressed()
{
  uint32_t prev = millis();
  while (digitalRead(15) == HIGH)
  {
    delay(50);
    if ((millis() - prev) > 5L * 1000L)
    {
      Serial.println("LONG PRESSED.");
      while (digitalRead(15) == HIGH)
      {
        delay(10);
      }
      SPIFFS.remove("/enabled");
      Serial.println("being restarted.");
      delay(1000);
      ESP.restart();
    }
  }
}

void WiFiModule::setup()
{
  u8g2 = new U8G2_ST7920_128X64_1_SW_SPI(U8G2_R0, /* clock=*/14, /* data=*/13, /* CS=*/12);
  u8g2->begin();
  _init_sta();
}

void WiFiModule::loop() {}
void WiFiModule::_init_sta()
{
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  delay(20);
  WiFi.mode(WIFI_STA);
  delay(20);
  WiFi.begin(sta_ssid, sta_pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
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
    Serial.printf("Connecting to %s:%s\r\n", sta_ssid, sta_pwd);
    isLongPressed();
    delay(300);
  }
  Serial.println("WiFi Connected.");
}

#include <CMMC_Module.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <CMMC_Interval.h>

#ifndef CMMC_NTP_MODULE_H
#define CMMC_NTP_MODULE_H 
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
class NTPModule: public CMMC_Module {
  public:
    void config(CMMC_System *os, AsyncWebServer* server); 
    void setup();
    void loop(); 
    void isLongPressed(); 
    void configLoop();
    void printTime();
  protected:
    void configWebServer();
    void sendNTPpacket(IPAddress& address);

  private:
    unsigned int localPort = 2390;
    IPAddress timeServerIP;
    const char* ntpServerName = "time.nist.gov"; 
    byte packetBuffer[ NTP_PACKET_SIZE]; 
    WiFiUDP udp;
    CMMC_Interval interval;
    CMMC_Interval interval2;
    unsigned long epoch;
    unsigned long unix;
    unsigned long lastUpdateMs;
};

#endif
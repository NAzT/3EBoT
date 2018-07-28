
#include <CMMC_Legend.h>
#include <CMMC_Sensor.h>

#include "modules/LCDModule.h"
#include "modules/NTPModule.h"
#include "modules/WiFiModule.h"
#include "modules/MqttModule.h"
#include "modules/SensorModule.h"

CMMC_Legend os;
LCDModule *lcdModule;
NTPModule *ntpModule;
SensorModule *sensorModule;

int temp;
void setup()
{ 
  lcdModule = new LCDModule();
  ntpModule = new NTPModule();
  sensorModule = new SensorModule();

  os.addModule(lcdModule); 
  os.addModule(new WiFiModule()); 
  os.addModule(ntpModule); 
  os.addModule(sensorModule);
  os.addModule(new MqttModule()); 
  os.setup();
  Serial.printf("APP VERSION: %s\r\n", LEGEND_APP_VERSION);
}

void loop()
{
  os.run();
}

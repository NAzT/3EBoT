
#include <CMMC_Legend.h>
#include <CMMC_Sensor.h>

#include <modules/WiFiModule.h>
#include <modules/MqttModule.h>

CMMC_Legend os;

int temp;

void setup()
{ 
  os.addModule(new WiFiModule()); 
  os.addModule(new MqttModule()); 
  os.setup();
  Serial.printf("APP VERSION: %s\r\n", LEGEND_APP_VERSION);
}

void loop()
{
  os.run();
}
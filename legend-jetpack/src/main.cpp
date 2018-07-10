
#include <CMMC_Legend.h>
#include <CMMC_Sensor.h>

#include <modules/WiFiModule.h>
#include <modules/MqttModule.h>

CMMC_Legend os;

int temp;

// 3EBot_1
// mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i j8pShiIYNIerG8n6 -u "1Xzlmdf3MNFjRgu" -P "2bugw3+nLiKqzxZxmIUkfEbCPtU=" -p 1883 -d
// 3EBot_2
// mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i 4bYQs345yXHQIGN5 -u "1Xzlmdf3MNFjRgu" -P "ubJkmJoT16BHDVoZzB8FmlMzsVU=" -p 1883 -d
// 3EBot_3
//  mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i jfuxjQQYxn6QsA4k -u "1Xzlmdf3MNFjRgu" -P "jYAuCFCTcfPETePxiZbOhDBZddo=" -p 1883 -d
// debug
// mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i LOuN74G9Hh0WHk3R -u "1Xzlmdf3MNFjRgu" -P "q+t0cKm1J4VJnzaQEtT4T1u38J8=" -p 1883 -d

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
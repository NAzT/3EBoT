
#include <CMMC_Legend.h>
#include <CMMC_Sensor.h>

#include <modules/WiFiModule.h>
#include <modules/MqttModule.h>

CMMC_Legend os;

int temp;

// 3EBOT1
// mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i j8pShiIYNIerG8n6 -u "1Xzlmdf3MNFjRgu" -P "2bugw3+nLiKqzxZxmIUkfEbCPtU=" -p 1883 -d
// 3EBOT2
// mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i 4bYQs345yXHQIGN5 -u "1Xzlmdf3MNFjRgu" -P "ubJkmJoT16BHDVoZzB8FmlMzsVU=" -p 1883 -d
// 3EBOT3
//  mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i jfuxjQQYxn6QsA4k -u "1Xzlmdf3MNFjRgu" -P "jYAuCFCTcfPETePxiZbOhDBZddo=" -p 1883 -d
// 3EBOT4
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i Jg1NORe62dT0c0z2 -u "1Xzlmdf3MNFjRgu" -P "rK7J8jHF+jBE6zJJyjltR0M4o5Q=" -p 1883 -d
// 3EBOT5
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i BhWVdZn5ztYqTiM2 -u "1Xzlmdf3MNFjRgu" -P "Hk0XGYHkiYL9Rvemxxq1/1i44MY=" -p 1883 -d
// 3EBOT6
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i BZK9PQGzlZVpFSGd -u "1Xzlmdf3MNFjRgu" -P "JW/fUZ2kybFR2J6jt++RpWS/7mg=" -p 1883 -d
// 3EBOT7
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i F1WorIo3uur55iWp -u "1Xzlmdf3MNFjRgu" -P "oB3Lo8HyaFhPt7WIiSz+7Cx2dtk=" -p 1883 -d
// 3EBOT8
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i IYengmXO7BMhXEsE -u "1Xzlmdf3MNFjRgu" -P "y3D5s99Y0ydAuSXD98P8EyJXmBk=" -p 1883 -d
// 3EBOT9
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i 2Mw8vVCBYahxQ7gJ -u "1Xzlmdf3MNFjRgu" -P "6OUcPsb9xB/LshR4j0+F98qnWHI=" -p 1883 -d
// 3EBOT10
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i grZaNj6fNe2qRX6E -u "1Xzlmdf3MNFjRgu" -P "x+at/MLnyeCZ/uueivshMZRZoaY=" -p 1883 -d



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

#include <CMMC_Legend.h>
#include <CMMC_Sensor.h>

#include <modules/WiFiModule.h>
#include <modules/MqttModule.h>

CMMC_Legend os;

int temp;

// Blue Display
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
// 3EBOT11
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i 8dVVh8hjprUApSiO -u "1Xzlmdf3MNFjRgu" -P "KNg28NgHTN+sBgc3V3/2dWwWmuU=" -p 1883 -d
// 3EBOT12
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i U7yjGmbKam9ZCjTy -u "1Xzlmdf3MNFjRgu" -P "5kXH8oLtDhr2/MOnwxNtyaYz74I=" -p 1883 -d
// 3EBOT13
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i lu9sZpHenXIQFSzp -u "1Xzlmdf3MNFjRgu" -P "H3RVBbPggUbF5qmz9arRDKZfhpE=" -p 1883 -d
// 3EBOT14
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i r6zPATe0ZDoE4Oh3 -u "1Xzlmdf3MNFjRgu" -P "eDsxXwsfGB9G5XZ/2TtaE50Ij8w=" -p 1883 -d
// 3EBOT15
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i PEsfeShz8I28zFGa -u "1Xzlmdf3MNFjRgu" -P "mYVYNolXaZXTRwjexNS0CyTP/BU=" -p 1883 -d
// 3EBOT16
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i llNmEMLuEurEXjXG -u "1Xzlmdf3MNFjRgu" -P "DNkjgVYnDbTk8UGBX/luzdBgEjo=" -p 1883 -d
// 3EBOT17
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i izba1JryJOGc5Ml3 -u "1Xzlmdf3MNFjRgu" -P "hbXfQpkOPnaaqDua1++2U0mrMYk=" -p 1883 -d
// 3EBOT18
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i eYBWbA5CVY3ttJ9V -u "1Xzlmdf3MNFjRgu" -P "CglOWqr9AK9jgkvrxvTk0W49Uqs=" -p 1883 -d
// 3EBOT19
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i uD2JzdLAs9DSEx2O -u "1Xzlmdf3MNFjRgu" -P "JQb3x6F54bTWxBMrLYRWvwKD0b0=" -p 1883 -d
// 3EBOT20
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i 5I2NXUE7Js3UymMs -u "1Xzlmdf3MNFjRgu" -P "dpZGcmDsjxZGV3vfGfcPSc55au4=" -p 1883 -d


// Yellow Display
// 3EBOT21
//mosquitto_sub -t "/3EBot/gearname/#" -h gb.netpie.io -i Qu2epqNd7gqDx41u -u "1Xzlmdf3MNFjRgu" -P "4/3E2qLB4h/OBmScAf+8FzYp4Nk=" -p 1883 -d

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
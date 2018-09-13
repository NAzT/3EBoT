#include "SensorModule.h"
#include "LCDModule.h"

extern LCDModule *lcdModule;

void swap(float &a, float &b)
{
  int t = a;
  a = b;
  b = t;
}

int partition(float *arr, const int left, const int right)
{
  const int mid = left + (right - left) / 2;
  const int pivot = arr[mid];
  // move the mid point value to the front.
  swap(arr[mid], arr[left]);
  int i = left + 1;
  int j = right;
  while (i <= j)
  {
    while (i <= j && arr[i] <= pivot)
    {
      i++;
    }

    while (i <= j && arr[j] > pivot)
    {
      j--;
    }

    if (i < j)
    {
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i - 1], arr[left]);
  return i - 1;
}

void quickSort(float *arr, const int left, const int right)
{
  if (left >= right)
  {
    return;
  }

  int part = partition(arr, left, right);

  quickSort(arr, left, part - 1);
  quickSort(arr, part + 1, right);
}

int median(float arr[], int maxValues)
{
  quickSort(arr, 0, maxValues - 1);
  return arr[maxValues / 2];
}

void SensorModule::configLoop()
{
}

void SensorModule::config(CMMC_System *os, AsyncWebServer *server)
{
  strcpy(this->path, "/api/sensors");
  this->_serverPtr = server;
  this->_managerPtr = new CMMC_ConfigManager("/sensors.json");
  this->_managerPtr->init();
  this->_managerPtr->load_config([&](JsonObject * root, const char *content) {
    if (root == NULL)
    {
      Serial.print("sensors.json failed. >");
      Serial.println(content);
      return;
    }
    else {
      Serial.println(content); 
      const char *soil_configs [] = {(*root)["soil_enable"],
                              (*root)["soil_max"], (*root)["soil_min"],
                              (*root)["soil_moisture"]
                              };
      soil_enable = String(soil_configs[0]).toInt();
      soil_max  = String(soil_configs[1]).toInt();
      soil_min = String(soil_configs[2]).toInt();
      soil_moisture = String(soil_configs[3]).toInt();
    }
  });
  this->configWebServer();
  // if (lcdModule)
  // {
  //   // lcdModule->displayConfigWiFi();
  // }
}

void SensorModule::configWebServer()
{
  _serverPtr->on(this->path, HTTP_POST, [&](AsyncWebServerRequest *request) {
    String output = saveConfig(request, this->_managerPtr);
    request->send(200, "application/json", output);
  });
}


float SensorModule::getAnalog(int slot)
{
  return _adc0[slot-1];
}

void SensorModule::setup()
{
  Wire.begin(4, 5);
  bme = new Adafruit_BME280();
  bool bmeStatus;
  bmeStatus = bme->begin(0x76);
  if (!bmeStatus) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  if (soil_enable) {
    pinMode(2, OUTPUT);
  }

  ads = new Adafruit_ADS1115(0x48); 
  ads->setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV 
  ads->setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads->begin(); 
}

void SensorModule::loop()
{
  interval.every_ms(2000, [&]() {
    int idx = counter % MAX_ARRAY;

    if (soil_enable) {
      int16_t adc0;
      adc0 = ads->readADC_SingleEnded(0);
      adc0_array[0][idx] = adc0; 
      Serial.printf("[ENABLE] soil sensor is enabled!\r\n");
    }
    else {
      Serial.printf("[DISABLE] soil sensor is not enabled!\r\n");
    }


    // int16_t adc1 = ads->readADC_SingleEnded(1);
    // int16_t adc2 = ads->readADC_SingleEnded(2);
    // int16_t adc3 = ads->readADC_SingleEnded(3);

    // Serial.printf("adc0 = %d\r\n", adc0);
    // Serial.printf("adc1 = %d\r\n", adc1);
    // Serial.printf("adc2 = %d\r\n", adc2);
    // Serial.printf("adc3 = %d\r\n", adc3);

    temp_array[0][idx] = bme->readTemperature();
    humid_array[0][idx] = bme->readHumidity();
    if (counter < MAX_ARRAY)
    {
      _temperature[0] = median(temp_array[0], idx + 1);
      _humidity[0] = median(humid_array[0], idx + 1);
      _pressure[0] = median(pressure_array[0], idx + 1);
      _adc0[0] = median(adc0_array[0], idx + 1);
    }
    else
    {
      _temperature[0] = median(temp_array[0], MAX_ARRAY);
      _humidity[0] = median(humid_array[0], MAX_ARRAY);
      _pressure[0] = median(pressure_array[0], MAX_ARRAY);
      _adc0[0] = median(adc0_array[0], MAX_ARRAY);
    }
    
    float a0_percent = map(_adc0[0], soil_min, soil_max, 100, 0);
    if (soil_enable) {
      soil_moisture_percent = a0_percent; 
      if (a0_percent <= soil_moisture) {
        digitalWrite(2, HIGH); 
      } 
      else {
        digitalWrite(2, LOW); 
      }
    }
    Serial.printf("temp=%.2f humid=%.2f, adc0=%.2f, %f%%\r\n", 
      this->_temperature[0], this->_humidity[0], this->_adc0[0], soil_moisture_percent); 
    counter++;
  });
}

String SensorModule::getTemperatureString(int slot)
{
  char buffer[10];
  sprintf(buffer, "%.1f", _temperature[slot-1]);
  return String(buffer);
}

String SensorModule::getHumidityString(int slot)
{
  return String((int)_humidity[slot-1]);
}

float SensorModule::getTemperature(int slot)
{
  return _temperature[slot-1];
}

float SensorModule::getHumidity(int slot)
{
  return _humidity[slot-1];
}

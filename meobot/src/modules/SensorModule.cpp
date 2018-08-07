#include "SensorModule.h"
#include "LCDModule.h"

extern LCDModule* lcdModule; 

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
  while (i <= j) {
    while (i <= j && arr[i] <= pivot) {
      i++;
    }

    while (i <= j && arr[j] > pivot) {
      j--;
    }

    if (i < j) {
      swap(arr[i], arr[j]);
    }
  }

  swap(arr[i - 1], arr[left]);
  return i - 1;
}

void quickSort(float *arr, const int left, const int right)
{
  if (left >= right) {
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

void SensorModule::configLoop() {
}

void SensorModule::config(CMMC_System *os, AsyncWebServer *server)
{
  if (lcdModule) {
    // lcdModule->displayConfigWiFi(); 
  }
}

void SensorModule::configWebServer()
{
  static SensorModule *that = this;
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
}

void SensorModule::loop() 
{ 
  interval.every_ms(5000, [&]() { 
     int idx = counter % MAX_ARRAY;
    temp_array[idx] = bme->readTemperature();
    humid_array[idx] = bme->readHumidity(); 
    if (counter < MAX_ARRAY) {
      Serial.println("CASE-1");
      _temperature = median(temp_array, idx+1);
      _humidity = median(humid_array, idx+1); 
      _pressure = median(pressure_array, idx+1);
    }
    else {
      Serial.println("CASE-2");
      _temperature = median(temp_array, MAX_ARRAY);
      _humidity = median(humid_array, MAX_ARRAY);
      _pressure = median(pressure_array, MAX_ARRAY);
    }
    Serial.printf("temp=%.2f humid=%.2f\r\n", _temperature, _humidity);
    counter++;
  });
}

String SensorModule::getTemperatureString() { 
  char buffer[10];
  sprintf(buffer,"%.1f", _temperature);
  return String(buffer); 
}

String SensorModule::getHumidityString() { 
  return String((int)_humidity); 
}
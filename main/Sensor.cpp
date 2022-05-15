
#include "Sensor.h"

#define DHTPIN 2    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define SOILMOISTURE A0

Sensor::Sensor()
  : dht(DHTPIN, DHTTYPE)
  , initialized(false)
{
  memset(caches, 0, sizeof(caches));
  memset(values, 0, sizeof(values));
  pinMode(SOILMOISTURE, INPUT);
  dht.begin();
}

void Sensor::run() 
{
  values[0] = dht.readTemperature();
  values[1] = dht.readHumidity();
  values[2] = analogRead(SOILMOISTURE);
  initialized = true; runned();
}

bool Sensor::temperature(float* value)
{
  *value = values[0];
  bool dirty = caches[0] != *value;
  caches[0] = *value;
  return dirty || !initialized;
}

bool Sensor::humidity(float* value)
{
  *value = values[1];
  bool dirty = caches[1] != *value;
  caches[1] = *value;
  return dirty || !initialized;
}

bool Sensor::moisture(int* value)
{
  *value = int(values[2]);
  bool dirty = int(caches[2]) != *value;
  caches[2] = *value;
  return dirty || !initialized;
}

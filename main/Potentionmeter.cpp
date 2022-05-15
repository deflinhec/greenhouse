#include <Arduino.h>
#include "Potentionmeter.h"
#define MOISTURE A1
#define TEMPERATURE A2
#define MAX_MOISTURE 400.0f
#define MAX_TEMPERATURE 50.0f

Potentiometer::Potentiometer()
  : initialized(false)
{
  pinMode(MOISTURE, INPUT);
  pinMode(TEMPERATURE, INPUT);
  memset(caches, 0, sizeof(caches));
  memset(values, 0, sizeof(values));
}

static float Ratio(int value)
{
  // stablized value
//  value += value % 10; 
  float r = float(value) / 1000.0f;
  return constrain(r, 0.0f, 1.0f);
}

void Potentiometer::run() 
{
  values[0] = analogRead(MOISTURE);
  Serial.println(values[0]);
  values[0] = Ratio(values[0]) * MAX_MOISTURE;
//  values[1] = analogRead(TEMPERATURE);
  values[1] = Ratio(values[1]) * MAX_TEMPERATURE;
  initialized = true; runned();
}

bool Potentiometer::moisture(int* value)
{
  *value = float(values[0]);
  bool dirty = caches[0] != *value;
  caches[0] = *value;
  return dirty || !initialized;
}
 
bool Potentiometer::temperature(float* value)
{
  *value = float(values[1]);
  bool dirty = caches[1] != int(*value);
  caches[1] = *value;
  return dirty || !initialized;
}

#pragma once
#include <DHT.h>
#include <Thread.h>
#include "Model.h"

class Sensor : public Thread, public Model
{
  void run();
public:

  Sensor();

  bool temperature(float* value);

  bool humidity(float* value);

  bool moisture(int* value);

private:
  DHT dht;
  float values[3];
  float caches[3];
  bool initialized;
};

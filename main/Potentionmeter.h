#pragma once

#include <Thread.h>
#include "Model.h"

class Potentiometer : public Thread, public Model
{
  void run();
public:

  Potentiometer();

  bool moisture(int* value);
 
  bool temperature(float* value);

private:
  int values[2];
  int caches[2];
  bool initialized;
};

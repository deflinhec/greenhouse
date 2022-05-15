#include "ModelControl.h"

bool ModelControl::select(Model* model)
{
  bool dirty = current != model;
  current = model;
  return dirty; 
}

Model* ModelControl::model()
{
  return current;
}

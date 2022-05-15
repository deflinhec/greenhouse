#pragma once

#include "Model.h"

class ModelControl
{
public:

  bool select(Model* model);

  Model* model();

private:
  Model* current;
};

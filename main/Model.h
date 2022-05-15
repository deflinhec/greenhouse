
#pragma once

class Model 
{
public: 

  virtual bool temperature(float* value) {return false;}

  virtual bool humidity(float* value) {return false;}

  virtual bool moisture(int* value) {return false;}
};

class ModelSwitch
{
public:

  bool select(Model* model);

  Model* model();

private:
  Model* current;
};

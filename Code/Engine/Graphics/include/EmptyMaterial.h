#pragma once
#include "Material.h"

class CEmptyMaterial : public CMaterial
{
public:
  CEmptyMaterial() {};
  virtual ~CEmptyMaterial() {};

  virtual void Activate() {};
  virtual float GetParallaxHeight() {return 0.0f;};
  virtual float GetBump() {return 0.0f;};
};


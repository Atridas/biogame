#pragma once
#include "Material.h"

class CEmptyMaterial : public CMaterial
{
public:
  CEmptyMaterial() {};
  virtual ~CEmptyMaterial() {};

  virtual void Activate() {};
  virtual float GetParallaxHeight()    const {return 0.0f;};
  virtual float GetBump()              const {return 0.0f;};
  virtual float GetGlossiness()        const {return 100.0f;};
  virtual float GetSpecularFactor()    const {return 0.1f;};
  virtual bool HasAlphaBlending()      const {return false; };
  virtual bool HasSpecularParameters() const {return false; };
};


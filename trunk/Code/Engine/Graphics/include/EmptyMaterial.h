#pragma once
#include "Material.h"

class CEmptyMaterial : public CMaterial
{
public:
  CEmptyMaterial() {};
  virtual ~CEmptyMaterial() {};

  virtual bool ReloadTextures()        const {return true;};

  virtual void Activate() {};
  virtual void Activate(int _iMaterialMask) {};
  virtual float GetParallaxHeight()    const {return 0.0f;};
  virtual float GetBump()              const {return 0.0f;};
  virtual float GetGlossiness()        const {return 100.0f;};
  virtual float GetSpecularFactor()    const {return 0.1f;};
  virtual float GetGlowIntensity()     const {return 1.0f;};

  virtual void SetGlowIntensity(const float& _fGlowIntensity) {};
  virtual void SetSpecularFactor(const float& _fSpecularFactor) {};
  virtual void SetGlossines(const float& _fGlossiness) {};
  virtual void SetBump(const float& _fBump) {};

  virtual bool HasAlphaBlending()      const {return false; };
  virtual bool HasSpecularParameters() const {return false; };
};


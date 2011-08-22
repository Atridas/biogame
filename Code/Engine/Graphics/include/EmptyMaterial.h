#pragma once
#include "Material.h"

class CEmptyMaterial : public CMaterial
{
public:
  CEmptyMaterial() {};
  virtual ~CEmptyMaterial() {};

  virtual bool ReloadTextures()        const {return true;};

  virtual void   Activate() {};
  virtual void   Activate(int _iMaterialMask) {};
  virtual float  GetParallaxHeight()       const {return 0.0f;};
  virtual float  GetBump()                 const {return 0.0f;};
  virtual float  GetGlossiness()           const {return 100.0f;};
  virtual float  GetSpecularFactor()       const {return 0.1f;};
  virtual float  GetGlowIntensity()        const {return 1.0f;};
  virtual float  GetEnvironmentIntensity() const {return 0.5f;};
  virtual Vect2f GetSpriteSize()          const {return Vect2f(1.f,1.f);};

  virtual void SetGlowIntensity(float _fGlowIntensity) {};
  virtual void SetSpecularFactor(float _fSpecularFactor) {};
  virtual void SetGlossines(float _fGlossiness) {};
  virtual void SetBump(float _fBump) {};
  virtual void SetEnvironmentIntensity(float _fEnvironmentIntensity) {};
  virtual void ActivateRadiosityNormal(ELightmapMode _eMode) {};
  virtual void SetSpriteSize(const Vect2f& _vSize) {};

  virtual bool HasAlphaBlending()      const {return false; };
  virtual bool HasSpecularParameters() const {return false; };
};


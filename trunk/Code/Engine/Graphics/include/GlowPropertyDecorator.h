#pragma once
#ifndef __GLOW_PROPERTY_DECORATOR__
#define __GLOW_PROPERTY_DECORATOR__

#include "PropertyDecorator.h"

class CGlowPropertyDecorator : public CPropertyDecorator
{
public:
  CGlowPropertyDecorator(CMaterial* _pDecoratedMaterial, float _fGlowIntensity)
    : CPropertyDecorator(_pDecoratedMaterial),m_fGlowIntensity(_fGlowIntensity) {};
  virtual ~CGlowPropertyDecorator() {};
  
  virtual float GetGlowIntensity() const { return m_fGlowIntensity; };
  virtual void SetGlowIntensity(float _fGlowIntensity) {m_fGlowIntensity = _fGlowIntensity;};

private:

  float m_fGlowIntensity;
};

#endif
#pragma once
#ifndef __ENVIRONMENT_PROPERTY_DECORATOR__
#define __ENVIRONMENT_PROPERTY_DECORATOR__

#include "PropertyDecorator.h"

class CEnvirondmentPropertyDecorator : public CPropertyDecorator
{
public:
  CEnvirondmentPropertyDecorator(CMaterial* _pDecoratedMaterial, float _fEnvironmentIntensity)
    : CPropertyDecorator(_pDecoratedMaterial),m_fEnvironmentIntensity(_fEnvironmentIntensity) {};
  virtual ~CEnvirondmentPropertyDecorator() {};
  
  virtual float GetEnvironmentIntensity() const { return m_fEnvironmentIntensity; };
  virtual void SetEnvironmentIntensity(float _fEnvironmentIntensity) {m_fEnvironmentIntensity = _fEnvironmentIntensity;};

private:

  float m_fEnvironmentIntensity;
};

#endif
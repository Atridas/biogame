#pragma once

#ifndef __SPECULAR_PROPERTY_DECORATOR__
#define __SPECULAR_PROPERTY_DECORATOR__

#include "PropertyDecorator.h"

class CSpecularPropertyDecorator : public CPropertyDecorator
{
public:
  CSpecularPropertyDecorator(CMaterial* _pDecoratedMaterial, float _fGlossiness, float _fSpecularLevel)
    : CPropertyDecorator(_pDecoratedMaterial),m_fGlossiness(_fGlossiness),m_fSpecularLevel(_fSpecularLevel) {};
  virtual ~CSpecularPropertyDecorator() {};
  
  virtual bool HasSpecularParameters() const { return true; };

private:
  float m_fGlossiness;
  float m_fSpecularLevel;
};

#endif

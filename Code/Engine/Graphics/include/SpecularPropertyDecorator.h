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
  
  virtual float GetGlossiness()        const {return m_fGlossiness;};
  virtual float GetSpecularFactor()    const {return m_fSpecularLevel;};

  virtual void SetSpecularFactor(float _fSpecularFactor) {m_fSpecularLevel = _fSpecularFactor;};
  virtual void SetGlossines(float _fGlossiness) {m_fGlossiness = _fGlossiness;};

private:
  float m_fGlossiness;
  float m_fSpecularLevel;
};

#endif

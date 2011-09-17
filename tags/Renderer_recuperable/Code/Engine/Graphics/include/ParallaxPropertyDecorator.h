#pragma once
#include "PropertyDecorator.h"

class CParallaxPropertyDecorator : public CPropertyDecorator
{
public:
  CParallaxPropertyDecorator(CMaterial* _pDecoratedMaterial, float _fParallaxHeight)
    : CPropertyDecorator(_pDecoratedMaterial),m_fParallaxHeight(_fParallaxHeight) {};
  virtual ~CParallaxPropertyDecorator() {};

  virtual float GetParallaxHeight() const {return m_fParallaxHeight;};
  virtual int GetMaterialType() const {return PARALLAX_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();};

private:
  float m_fParallaxHeight;
};


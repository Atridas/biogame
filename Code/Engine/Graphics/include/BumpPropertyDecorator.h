#pragma once
#include "PropertyDecorator.h"

class CBumpPropertyDecorator : public CPropertyDecorator
{
public:
  CBumpPropertyDecorator(CMaterial* _pDecoratedMaterial, float _fBump)
    : CPropertyDecorator(_pDecoratedMaterial),m_fBump(_fBump) {};
  virtual ~CBumpPropertyDecorator() {};

  virtual float GetBump() const {return m_fBump;};

private:
  float m_fBump;
};


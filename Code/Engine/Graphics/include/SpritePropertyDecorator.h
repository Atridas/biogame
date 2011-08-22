#pragma once

#ifndef __SPRITE_PROPERTY_DECORATOR__
#define __SPRITE_PROPERTY_DECORATOR__

#include "PropertyDecorator.h"

class CSpritePropertyDecorator : public CPropertyDecorator
{
public:
  CSpritePropertyDecorator(CMaterial* _pDecoratedMaterial, const Vect2f& _vSize)
    : CPropertyDecorator(_pDecoratedMaterial),m_vSize(_vSize) {};
  virtual ~CSpritePropertyDecorator() {};

  virtual Vect2f GetSpriteSize()       const {return m_vSize;};
  
  virtual void SetSpriteSize(const Vect2f& _vSize) {m_vSize = _vSize;};

private:
  Vect2f m_vSize;
};

#endif

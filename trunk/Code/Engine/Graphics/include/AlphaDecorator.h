#pragma once
#ifndef __ALHPA_DECORATOR__
#define __ALHPA_DECORATOR__

#include "PropertyDecorator.h"

class CAlphaPropertyDecorator : public CPropertyDecorator
{
public:
  CAlphaPropertyDecorator(CMaterial* _pDecoratedMaterial)
    : CPropertyDecorator(_pDecoratedMaterial) {};
  virtual ~CAlphaPropertyDecorator() {};

  virtual bool HasAlphaBlending() const { return true; };

private:
};

#endif

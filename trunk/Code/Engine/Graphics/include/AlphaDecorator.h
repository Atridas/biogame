#pragma once
#ifndef __ALHPA_DECORATOR__
#define __ALHPA_DECORATOR__

#include "PropertyDecorator.h"

class CAlphaDecorator : public CPropertyDecorator
{
public:
  CAlphaDecorator(CMaterial* _pDecoratedMaterial)
    : CPropertyDecorator(_pDecoratedMaterial) {};
  virtual ~CAlphaDecorator() {};
  
  virtual int GetMaterialType() const {return m_pDecoratedMaterial->GetMaterialType();};

  virtual bool HasAlphaBlending() const { return true; };

private:
};

#endif

#pragma once
#include "MaterialDecorator.h"

class CPropertyDecorator : public CMaterialDecorator
{
public:
  CPropertyDecorator(CMaterial* _pDecoratedMaterial)
    : CMaterialDecorator(_pDecoratedMaterial) {};

  virtual ~CPropertyDecorator() {};

  virtual void Activate() {m_pDecoratedMaterial->Activate();};
  virtual float GetParallaxHeight() {return m_pDecoratedMaterial->GetParallaxHeight();};
  virtual float GetBump() {return m_pDecoratedMaterial->GetBump();};
  virtual int GetMaterialType() {return m_pDecoratedMaterial->GetMaterialType();};
};


#pragma once
#include "MaterialDecorator.h"

class CPropertyDecorator : public CMaterialDecorator
{
public:
  CPropertyDecorator(CMaterial* _pDecoratedMaterial)
    : CMaterialDecorator(_pDecoratedMaterial) {};

  virtual ~CPropertyDecorator() {};

  virtual void Activate() {m_pDecoratedMaterial->Activate();};
  virtual int GetMaterialType() {return m_pDecoratedMaterial->GetMaterialType();};
};


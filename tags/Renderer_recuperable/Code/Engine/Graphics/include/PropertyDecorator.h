#pragma once
#include "MaterialDecorator.h"

class CPropertyDecorator : public CMaterialDecorator
{
public:
  CPropertyDecorator(CMaterial* _pDecoratedMaterial)
    : CMaterialDecorator(_pDecoratedMaterial) {};

  virtual ~CPropertyDecorator() {};

  virtual void Activate() {m_pDecoratedMaterial->Activate();};
  virtual void Activate(int _iTextureMask) {m_pDecoratedMaterial->Activate(_iTextureMask);};
  virtual int GetMaterialType() const {return m_pDecoratedMaterial->GetMaterialType();};
};


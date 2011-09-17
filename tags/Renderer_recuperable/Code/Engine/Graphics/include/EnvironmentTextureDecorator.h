#pragma once
#include "TextureDecorator.h"

class CEnvironmentTextureDecorator : public CTextureDecorator
{
public:
  CEnvironmentTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CTextureDecorator(_pDecoratedMaterial,_pTexture) {};
  ~CEnvironmentTextureDecorator() {};

  virtual int GetMaterialType() const {return ENVIRONMENT_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();};
  virtual void Activate() {m_pTexture->Activate(6);m_pDecoratedMaterial->Activate();};
  virtual void Activate(int _iTextureMask) {(_iTextureMask & ENVIRONMENT_MATERIAL_MASK)? Activate() : m_pDecoratedMaterial->Activate();};
};
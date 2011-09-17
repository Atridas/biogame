#pragma once
#include "TextureDecorator.h"

class CSpecularTextureDecorator : public CTextureDecorator
{
public:
  CSpecularTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CTextureDecorator(_pDecoratedMaterial,_pTexture) {};
  ~CSpecularTextureDecorator() {};

  virtual int GetMaterialType() const {return SPECULARMAP_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();};
  virtual void Activate() {m_pTexture->Activate(3);m_pDecoratedMaterial->Activate();};
  virtual void Activate(int _iTextureMask) {(_iTextureMask & SPECULARMAP_MATERIAL_MASK)? Activate() : m_pDecoratedMaterial->Activate();};
};


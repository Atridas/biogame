#pragma once
#include "TextureDecorator.h"

class CNormalTextureDecorator : public CTextureDecorator
{
public:
  CNormalTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CTextureDecorator(_pDecoratedMaterial,_pTexture) {};
  ~CNormalTextureDecorator() {};

  virtual int GetMaterialType() const {return NORMALMAP_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();};
  virtual void Activate() {m_pTexture->Activate(1);m_pDecoratedMaterial->Activate();};
  virtual void Activate(int _iTextureMask) {(_iTextureMask & NORMALMAP_MATERIAL_MASK)? Activate() : m_pDecoratedMaterial->Activate();};
};


#pragma once
#include "TextureDecorator.h"

class CDiffuseTextureDecorator : public CTextureDecorator
{
public:
  CDiffuseTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CTextureDecorator(_pDecoratedMaterial,_pTexture) {};
  ~CDiffuseTextureDecorator() {};

  virtual int GetMaterialType() const {return DIFFUSE_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();};
  virtual void Activate() {m_pTexture->Activate(0);m_pDecoratedMaterial->Activate();};
  virtual void Activate(int _iTextureMask) {(_iTextureMask & DIFFUSE_MATERIAL_MASK)? Activate() : m_pDecoratedMaterial->Activate();};
};


#pragma once
#include "TextureDecorator.h"

class CGlowTextureDecorator : public CTextureDecorator
{
public:
  CGlowTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CTextureDecorator(_pDecoratedMaterial,_pTexture) {};
  ~CGlowTextureDecorator() {};

  virtual int GetMaterialType() {return GLOW_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();};
  virtual void Activate() {m_pTexture->Activate(4);m_pDecoratedMaterial->Activate();};
};


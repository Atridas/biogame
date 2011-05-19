#pragma once
#include "TextureDecorator.h"

class CLightmapTextureDecorator : public CTextureDecorator
{
public:
  CLightmapTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CTextureDecorator(_pDecoratedMaterial,_pTexture) {};
  ~CLightmapTextureDecorator() {};

  virtual int GetMaterialType() const {return LIGHTMAP_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();};
  virtual void Activate() {m_pTexture->Activate(2);m_pDecoratedMaterial->Activate();};
};


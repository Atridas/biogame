#pragma once
#include "MaterialDecorator.h"
#include "Texture.h"

class CTextureDecorator : public CMaterialDecorator
{
public:
  CTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CMaterialDecorator(_pDecoratedMaterial), m_pTexture(_pTexture) {};

  virtual ~CTextureDecorator() {};

  virtual bool ReloadTextures()        const { return m_pTexture->Reload() && m_pDecoratedMaterial->ReloadTextures();};

protected:
  CTexture* m_pTexture;
};
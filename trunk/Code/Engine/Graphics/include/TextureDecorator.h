#pragma once
#include "MaterialDecorator.h"
#include "Texture.h"

class CTextureDecorator : public CMaterialDecorator
{
public:
  CTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CMaterialDecorator(_pDecoratedMaterial), m_pTexture(_pTexture) {};

  virtual ~CTextureDecorator() {};

protected:
  CTexture* m_pTexture;
};


#pragma once
#include "MaterialDecorator.h"
#include "Texture.h"

class CTextureDecorator : public CMaterialDecorator
{
public:
  CTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CMaterialDecorator(_pDecoratedMaterial), m_pTexture(_pTexture) {};

  virtual ~CTextureDecorator() {};

  virtual float GetParallaxHeight() {return m_pDecoratedMaterial->GetParallaxHeight();};
  virtual float GetBump() {return m_pDecoratedMaterial->GetBump();};

protected:
  CTexture* m_pTexture;
};


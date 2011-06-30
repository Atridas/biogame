#pragma once
#include "TextureDecorator.h"

class CLightmapTextureDecorator : public CTextureDecorator
{
public:
  CLightmapTextureDecorator(CMaterial* _pDecoratedMaterial,CTexture* _pTexture)
    : CTextureDecorator(_pDecoratedMaterial,_pTexture), m_pTextureR(0), m_pTextureG(0), m_pTextureB(0) {};
  ~CLightmapTextureDecorator() {};

  virtual int GetMaterialType() const;
  virtual void Activate();
  virtual void Activate(int _iTextureMask);

  virtual void ActivateRadiosityNormal();

private:
  CTexture* m_pTextureR;
  CTexture* m_pTextureG;
  CTexture* m_pTextureB;
};


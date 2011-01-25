#pragma once
#ifndef __RENDER_TEXTURE_SCENE_EFFECT_H__
#define __RENDER_TEXTURE_SCENE_EFFECT_H__

#include "base.h"
#include "SceneEffect.h"
#include <d3d9.h>

//Forward declarations-------------
class MKeyValue;
class CRenderManager;
class CTexture;
//---------------------------------

class CRenderTextureSceneEffect :
  public CSceneEffect
{
public:
  CRenderTextureSceneEffect(): m_pTexture(0), m_pSurface(0) {};
  ~CRenderTextureSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);

  void CaptureFrameBuffers(CRenderManager* _pRM);

protected:
  CTexture* m_pTexture;
  LPDIRECT3DSURFACE9 m_pSurface;

  void Release();
};

#endif

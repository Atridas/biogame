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
  public  CBaseControl, CSceneEffect
{
public:
  CRenderTextureSceneEffect(MKeyValue& _Atts);
  ~CRenderTextureSceneEffect() {Done();};

  void CaptureFrameBuffers(CRenderManager* _pRM);

protected:
  CTexture* m_pTexture;
  LPDIRECT3DSURFACE9 m_pSurface;
};

#endif

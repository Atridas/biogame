#pragma once
#ifndef __RENDER_TO_TEXTURE_SCENE_EFFECT_H__
#define __RENDER_TO_TEXTURE_SCENE_EFFECT_H__

#include "base.h"
#include "RenderTextureSceneEffect.h"

//Forward declarations-------------
class MKeyValue;
class CRenderManager;
class CProcess;
class CEffectTechnique;
class CEffect;
//---------------------------------

class CRenderToTextureSceneEffect :
  public CRenderTextureSceneEffect
{
public:
  CRenderToTextureSceneEffect():
                m_pStaticMeshEffect(0),
                m_pAnimatedModelEffect(0)
                {};
  ~CRenderToTextureSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);

  void PreRender(CRenderManager* _pRM, CProcess* _pProc);

protected:
  CEffect* m_pStaticMeshEffect;
  CEffect* m_pAnimatedModelEffect;

  void Release();
};

#endif
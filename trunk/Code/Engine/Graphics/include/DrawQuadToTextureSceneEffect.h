#pragma once
#ifndef __DRAW_QUAD_TO_TEXTURE_SCENE_EFFECT_H__
#define __DRAW_QUAD_TO_TEXTURE_SCENE_EFFECT_H__

#include "RenderTextureSceneEffect.h"

// Forward declarations --------
class CEffect;
class CProcess;
class CRenderManager;
// -----------------------------

class CDrawQuadToTextureSceneEffect:
  public CRenderTextureSceneEffect
{
public:
  CDrawQuadToTextureSceneEffect():m_pEffect(0)
                                  {};
  ~CDrawQuadToTextureSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);

  virtual void PreRender(CRenderManager* _pRM, CProcess* _pProc);
  virtual void PostRender(CRenderManager* _pRM);

protected:
  void Release();

  CEffect* m_pEffect;
  CColor m_Color;

private:

  void Process(CRenderManager* _pRM);
};

#endif

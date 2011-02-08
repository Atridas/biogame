#pragma once
#ifndef __DRAW_QUAD_TO_TEXTURE_SCENE_EFFECT_H__
#define __DRAW_QUAD_TO_TEXTURE_SCENE_EFFECT_H__

#include "RenderTextureSceneEffect.h"

// Forward declarations --------
class CEffectTechnique;
class CProcess;
class CRenderManager;
// -----------------------------

class CDrawQuadToTextureSceneEffect:
  public CRenderTextureSceneEffect
{
public:
  CDrawQuadToTextureSceneEffect():m_pTechnique(0)
                                  {};
  ~CDrawQuadToTextureSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);

  virtual void PreRender(CRenderManager* _pRM, CProcess* _pProc);

protected:
  void Release();

  CEffectTechnique* m_pTechnique;
  CColor m_Color;
};

#endif

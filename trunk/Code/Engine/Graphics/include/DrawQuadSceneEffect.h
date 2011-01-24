#pragma once
#ifndef __DRAW_QUAD_SCENE_EFFECT_H__
#define __DRAW_QUAD_SCENE_EFFECT_H__

#include "SceneEffect.h"
#include "base.h"

//Forward Declarations ------------------------------------
class MKeyValue;
class CEffectTechnique;
//--------------------------------------------------------

class CDrawQuadSceneEffect : public CSceneEffect
{
public:
  CDrawQuadSceneEffect(): m_pTechnique(0) {};
  ~CDrawQuadSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);


  void PostRender(CRenderManager *RM);
protected:
  void Release();
  CEffectTechnique *m_pTechnique;
  CColor m_Color;
};

#endif

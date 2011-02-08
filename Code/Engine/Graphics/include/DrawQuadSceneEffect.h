#pragma once
#ifndef __DRAW_QUAD_SCENE_EFFECT_H__
#define __DRAW_QUAD_SCENE_EFFECT_H__

#include "SceneEffect.h"
#include "base.h"

//Forward Declarations ------------------------------------
class MKeyValue;
class CEffectTechnique;
class CEffectMaterial;
class CTexture;
//--------------------------------------------------------

class CDrawQuadSceneEffect : public CSceneEffect
{
public:
  CDrawQuadSceneEffect(): m_pTechnique(0),m_pEffectMaterial(0),m_szTechnique("") {};
  ~CDrawQuadSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);


  void PostRender(CRenderManager *RM);
protected:
  void Release();
  CEffectTechnique *m_pTechnique;
  string            m_szTechnique;
  CEffectMaterial*  m_pEffectMaterial;
  CColor m_Color;
};

#endif

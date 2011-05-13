#pragma once
#ifndef __DRAW_QUAD_SCENE_EFFECT_H__
#define __DRAW_QUAD_SCENE_EFFECT_H__

#include "SceneEffect.h"
#include "base.h"

//Forward Declarations ------------------------------------
class MKeyValue;
class CEffectTechnique;
class CEffect;
class CTexture;
//--------------------------------------------------------

class CDrawQuadSceneEffect : public CSceneEffect
{
public:
  CDrawQuadSceneEffect(): m_pEffect(0)/*,m_pEffectMaterial(0)*/,m_szEffect("") {};
  ~CDrawQuadSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);


  void PostRender(CRenderManager *RM);
protected:
  void Release();
  CEffect *m_pEffect;
  string            m_szEffect;
  //CEffectMaterial*  m_pEffectMaterial;
  int m_iWidth;
  int m_iHeight;
  CColor m_Color;
};

#endif

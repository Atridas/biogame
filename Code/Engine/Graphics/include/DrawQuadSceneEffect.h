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
  CDrawQuadSceneEffect(): m_pEffect(0),m_iPos(0),m_iSize(0),m_fAlphaFactor(1.0),m_szEffect(""),m_Alignment(UPPER_LEFT),m_fTime(0.0),m_bUseTime(false) {};
  ~CDrawQuadSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);

  void SetAlphaFactor(float _fAlphaFactor) {m_fAlphaFactor = _fAlphaFactor;};
  void SetTime(float _fTime) {m_fTime = _fTime;};

  void PostRender(CRenderManager *RM);
protected:
  void Release();
  CEffect *m_pEffect;
  string m_szEffect;

  Vect2i m_iSize;
  Vect2i m_iPos;

  ETypeAlignment m_Alignment;

  bool m_bUseTime;
  float m_fTime;
  
  CColor m_Color;
  float m_fAlphaFactor;
};

#endif

#pragma once
#ifndef __DRAW_QUAD_SCENE_EFFECT_H__
#define __DRAW_QUAD_SCENE_EFFECT_H__

#include "SceneEffect.h"
#include "base.h"

//Forward Declarations ------------------------------------
class MKeyValue;
class CEffectTechnique;
class CTexture;
//--------------------------------------------------------

class CDrawQuadSceneEffect : public CSceneEffect
{
public:
  struct QuadTextures {
    uint32 stage;
    CTexture* texture;
    QuadTextures(uint32 _stage, CTexture* _texture):stage(_stage),texture(_texture){};
  };

public:
  CDrawQuadSceneEffect(): m_pTechnique(0) {};
  ~CDrawQuadSceneEffect() {Done();};

  virtual bool Init(const CXMLTreeNode& _params);


  void PostRender(CRenderManager *RM);
protected:
  void Release();
  CEffectTechnique *m_pTechnique;
  CColor m_Color;
  vector<QuadTextures> m_vTextures;
};

#endif

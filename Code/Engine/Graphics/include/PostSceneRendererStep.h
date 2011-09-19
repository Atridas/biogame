#pragma once
#ifndef __POST_SCENE_RENDERER_STEP_H__
#define __POST_SCENE_RENDERER_STEP_H__

#include "RendererStep.h"
#include "RenderManager.h"

class CXMLTreeNode;
class CRenderManager;

class CPostSceneRendererStep :
  public CRendererStep
{
public:
  CPostSceneRendererStep() : m_szEffect(""),m_fTime(0.0f),m_bUseTime(false),m_iPos(0),m_iSize(0),m_fAlphaFactor(1.0),m_Alignment(UPPER_LEFT) {};
  ~CPostSceneRendererStep() {Done();};

  virtual bool Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget);

  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera) {Render(_pRM);};
  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera,
                      const vector<CObject3DRenderable*>& _vOpaqueObjects,
                      const vector<CObject3DRenderable*>& _vAlphaObjects,
                      const vector<CObject3DRenderable*>& _vParticleEmiters)
                     {Render(_pRM, _pCamera);};

  void SetTime(float _fTime) {m_fTime = _fTime;};
  void SetAlpha(float _fAlpha) {m_fAlphaFactor = _fAlpha;};

protected:
  virtual void Render(CRenderManager* _pRM);

  virtual void Release();

  string m_szEffect;

  float m_fTime;
  bool m_bUseTime;
  float m_fAlphaFactor;
  Vect2i m_iSize;
  Vect2i m_iPos;
  ETypeAlignment m_Alignment;
};

#endif
#pragma once
#ifndef __RENDERER_STEP_H__
#define __RENDERER_STEP_H__

#include "base.h"
#include "Activable.h"
#include "Named.h"
#include "Utils/BaseControl.h"

#include <d3d9.h>

//class CRenderTarget;
class CInputSampler;
class CXMLTreeNode;
class CRenderManager;
class CCamera;
class CObject3DRenderable;

class CRendererStep :
  public CActivable, public CBaseControl, public CNamed
{

public:
  CRendererStep() : CNamed(""), m_szRenderTarget(""),
                    m_bClearColor(false), m_bClearDepth(false), m_bClearStencil(false),
                    m_bRenderOpaque(true), m_bRenderAlphas(true)
                    {};
  virtual ~CRendererStep() {Done();};

  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera) = 0;

  const string& GetRenderTarget() {return m_szRenderTarget;};

  void ClearBuffer(CRenderManager* l_pRM) const;

  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera,
                      const vector<CObject3DRenderable*>& _vOpaqueObjects,
                      const vector<CObject3DRenderable*>& _vAlphaObjects,
                      const vector<CObject3DRenderable*>& _vParticleEmiters);

protected:
  virtual void SetViewProjectionMatrices(CRenderManager* _pRM, CCamera* _pCamera);

  //int m_iRenderTargetWidth;
  //int m_iRenderTargetHeight;

  virtual void Release();
  
  bool Init(CXMLTreeNode& _treeSceneRenderer, const string& _szDefaultRenderTarget);

  bool InitInputSamplers(CXMLTreeNode& _treeInputSamplers);

  void ActivateInputSamplers();
  void DeactivateInputSamplers() {};
  
  virtual void RenderObject3DRenderable(CRenderManager* _pRM, CObject3DRenderable* _pO3DRenderable) const {};
  virtual void RenderEmiter(CRenderManager* _pRM, CObject3DRenderable* _pO3DRenderable) const {};
  
  bool m_bRenderOpaque, m_bRenderAlphas;
private:
  
  string m_szRenderTarget;

  //vector<CRenderTarget*> m_vRenderTargets;

  vector<CInputSampler*> m_vInputSamplers;


  bool m_bClearColor, m_bClearDepth, m_bClearStencil;
  D3DCOLOR m_Color;
  float    m_fDepth;
  uint32   m_iStencil;
};

#endif

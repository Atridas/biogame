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

class CRendererStep :
  public CActivable, public CBaseControl, public CNamed
{

public:
  CRendererStep() : CNamed(""), m_szRenderTarget(""),
                    m_bClearColor(false), m_bClearDepth(false), m_bClearStencil(false)
                    {};
  virtual ~CRendererStep() {Done();};

  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera) = 0;

  const string& GetRenderTarget() {return m_szRenderTarget;};

  void ClearBuffer(CRenderManager* l_pRM) const;

protected:

  //int m_iRenderTargetWidth;
  //int m_iRenderTargetHeight;

  virtual void Release();
  
  bool Init(CXMLTreeNode& _treeSceneRenderer, const string& _szDefaultRenderTarget);

  bool InitInputSamplers(CXMLTreeNode& _treeInputSamplers);

  void ActivateInputSamplers();
  void DeactivateInputSamplers() {};

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

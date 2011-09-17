#pragma once
#ifndef __RENDERER_STEP_H__
#define __RENDERER_STEP_H__

#include "base.h"
#include "Activable.h"
#include "Named.h"
#include "Utils/BaseControl.h"

class CRenderTarget;
class CInputSampler;
class CXMLTreeNode;
class CRenderManager;
class CCamera;

class CRendererStep :
  public CActivable, public CBaseControl, public CNamed
{

public:
  CRendererStep() : CNamed(""),m_iRenderTargetWidth(0),m_iRenderTargetHeight(0) {};
  virtual ~CRendererStep() {Done();};

  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera) = 0;

protected:

  int m_iRenderTargetWidth;
  int m_iRenderTargetHeight;

  virtual void Release();

  bool InitRenderTargets(CXMLTreeNode& _treeRenderTargets);
  bool InitInputSamplers(CXMLTreeNode& _treeInputSamplers);

  void ActivateInputSamplers();
  void ActivateRenderTargets(CRenderManager* l_pRM);

  void DeactivateInputSamplers() {};
  void DeactivateRenderTargets(CRenderManager* l_pRM);

private:
  vector<CRenderTarget*> m_vRenderTargets;
  vector<CInputSampler*> m_vInputSamplers;
};

#endif

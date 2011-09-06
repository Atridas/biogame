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
class CProcess;

class CRendererStep :
  public CActivable, public CBaseControl, public CNamed
{

public:
  CRendererStep() : CNamed(""),m_iRenderTargetWidth(0),m_iRenderTargetHeight(0) {};
  virtual ~CRendererStep() {Done();};

  virtual void Render(CProcess* _pProcess) = 0;

protected:

  int m_iRenderTargetWidth;
  int m_iRenderTargetHeight;

  virtual void Release();

  bool InitRenderTargets(CXMLTreeNode& _treeRenderTargets);
  bool InitInputSamplers(CXMLTreeNode& _treeInputSamplers);

  void ActivateInputSamplers();
  void ActivateRenderTargets();

  void DeactivateInputSamplers() {};
  void DeactivateRenderTargets();

private:
  vector<CRenderTarget*> m_vRenderTargets;
  vector<CInputSampler*> m_vInputSamplers;
};

#endif

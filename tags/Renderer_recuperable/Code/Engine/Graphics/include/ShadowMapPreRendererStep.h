#pragma once
#ifndef __SHADOW_MAP_PRE_SCENE_RENDERER_STEP_H__
#define __SHADOW_MAP_PRE_SCENE_RENDERER_STEP_H__

#include "PreSceneRendererStep.h"
#include "FPSCamera.h"

class CXMLTreeNode;
class CLight;

class CShadowMapPreRendererStep :
  public CPreSceneRendererStep
{
public:
  CShadowMapPreRendererStep() : m_pLightShadowCast(0) {};
  virtual ~CShadowMapPreRendererStep() {Done();};

  bool Init(CXMLTreeNode& _treePreSceneRenderer);

  virtual void SetViewProjectionMatrices(CRenderManager* _pRM);
  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera);

  void SetShadowMapLightCast(CLight* _pLight) {m_pLightShadowCast = _pLight;};

protected:
  virtual void Release();

private:

  CLight* m_pLightShadowCast;

  CFPSCamera m_FPSCamera;
};

#endif


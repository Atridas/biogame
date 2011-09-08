#pragma once
#ifndef __SHADOW_MAP_PRE_SCENE_RENDERER_STEP_H__
#define __SHADOW_MAP_PRE_SCENE_RENDERER_STEP_H__

#include "PreSceneRendererStep.h"

class CXMLTreeNode;
class CLight;

class CShadowMapPreRendererStep :
  public CPreSceneRendererStep
{
public:
  CShadowMapPreRendererStep() : m_pLightShadowCast(0) {};
  virtual ~CShadowMapPreRendererStep() {Done();};

  bool Init(CXMLTreeNode& _treePreSceneRenderer);

  virtual void Render(CProcess* _pProcess);

  void SetShadowMapLightCast(CLight* _pLight) {m_pLightShadowCast = _pLight;};

protected:
  virtual void Release();

private:

  CLight* m_pLightShadowCast;
};

#endif

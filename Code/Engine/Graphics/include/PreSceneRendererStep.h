#pragma once
#ifndef __PRE_SCENE_RENDERER_STEP_H__
#define __PRE_SCENE_RENDERER_STEP_H__

#include "SceneRendererStep.h"

class CXMLTreeNode;

class CPreSceneRendererStep :
  public CSceneRendererStep
{
public:
  CPreSceneRendererStep() {};
  ~CPreSceneRendererStep() {Done();};

  bool Init(CXMLTreeNode& _treePreSceneRenderer, const string& _szDefaultRenderTarget);

protected:
  virtual void Release();
};

#endif

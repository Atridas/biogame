#pragma once
#ifndef __DEFERRED_POST_SCENE_RENDERER_STEP_H_
#define __DEFERRED_POST_SCENE_RENDERER_STEP_H_

#include "PostSceneRendererStep.h"

class CDeferredPostSceneRendererStep :
  public CPostSceneRendererStep
{
public:
  CDeferredPostSceneRendererStep() {};
  virtual ~CDeferredPostSceneRendererStep() {Done();};

  bool Init(CXMLTreeNode& _treePostSceneRenderer);

protected:
  virtual void Render(CRenderManager* _pRM);

  virtual void Release();

};

#endif
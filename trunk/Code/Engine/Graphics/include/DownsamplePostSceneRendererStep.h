#pragma once
#ifndef __DOWNSAMPLE_POST_SCENE_RENDERER_STEP_H__
#define __DOWNSAMPLE_POST_SCENE_RENDERER_STEP_H__

#include "PostSceneRendererStep.h"

class CXMLTreeNode;
class CRenderManager;

class CDownsamplePostSceneRendererStep :
  public CPostSceneRendererStep
{
public:
  ~CDownsamplePostSceneRendererStep() {Done();};
  
  virtual bool Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget);

protected:
  virtual void Render(CRenderManager* _pRM);

  virtual void Release();

  string m_szEffect;
};

#endif
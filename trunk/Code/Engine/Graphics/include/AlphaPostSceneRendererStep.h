#pragma once
#ifndef __ALPHA_POST_SCENE_RENDERER_STEP_H__
#define __ALPHA_POST_SCENE_RENDERER_STEP_H__

#include "PostSceneRendererStep.h"

class CXMLTreeNode;
class CRenderManager;

class CAlphaPostSceneRendererStep :
  public CPostSceneRendererStep
{
public:
  virtual bool Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget);
  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera,
                      const vector<CObject3DRenderable*>& _vOpaqueObjects,
                      const vector<CObject3DRenderable*>& _vAlphaObjects,
                      const vector<CObject3DRenderable*>& _vParticleEmiters)
                     {CRendererStep::Render(_pRM, _pCamera,_vOpaqueObjects,_vAlphaObjects,_vParticleEmiters);};
};

#endif
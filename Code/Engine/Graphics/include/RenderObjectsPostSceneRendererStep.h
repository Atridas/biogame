#pragma once
#ifndef __RENDER_OBJECTS_POST_SCENE_RENDERER_STEP_H__
#define __RENDER_OBJECTS_POST_SCENE_RENDERER_STEP_H__

#include "PostSceneRendererStep.h"

class CXMLTreeNode;
class CRenderManager;
class CSceneRendererStep;

class CRenderObjectsPostSceneRendererStep :
  virtual public CPostSceneRendererStep
{
public:
  CRenderObjectsPostSceneRendererStep(): m_pSceneRendererStep(0) {};
  ~CRenderObjectsPostSceneRendererStep() {Done();};

  virtual bool Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget);
  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera,
                      const vector<CObject3DRenderable*>& _vOpaqueObjects,
                      const vector<CObject3DRenderable*>& _vAlphaObjects,
                      const vector<CObject3DRenderable*>& _vParticleEmiters);

  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera) {assert(!"cridar aquesta funcio es un error");};
protected:
  virtual void Release();

private:
  CSceneRendererStep* m_pSceneRendererStep;
};

#endif
#pragma once
#ifndef __SCENE_RENDERER_STEP_H__
#define __SCENE_RENDERER_STEP_H__

#include "base.h"
#include "RendererStep.h"

class CXMLTreeNode;
class CRenderableObject;
class CRenderableAnimatedInstanceModel;
class CRenderManager;
class CCamera;

class CSceneRendererStep :
  public CRendererStep
{

public:
  CSceneRendererStep() : m_szStaticMeshEffect(""),m_szAnimatedModelEffect(""),m_bUseMap(false) {};
  ~CSceneRendererStep() {Done();};

  bool Init(CXMLTreeNode& _treeSceneRenderer, const string& _szDefaultRenderTarget);

  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera);

  virtual void Render(CRenderManager* _pRM, CCamera* _pCamera,
                      const vector<CObject3DRenderable*>& _vOpaqueObjects,
                      const vector<CObject3DRenderable*>& _vAlphaObjects,
                      const vector<CObject3DRenderable*>& _vParticleEmiters)
                     {CRendererStep::Render(_pRM, _pCamera, _vOpaqueObjects, _vAlphaObjects, _vParticleEmiters);};

protected:
  virtual void Release();

  map<int,string> m_mapMaterialEffects;
  string m_szStaticMeshEffect;
  string m_szAnimatedModelEffect;
  string m_szParticleEffect;
  bool m_bUseMap;

  //CCamera* m_pCamera;
  virtual void RenderObject3DRenderable(CRenderManager* _pRM, CObject3DRenderable* _pO3DRenderable) const;
  virtual void RenderEmiter(CRenderManager* _pRM, CObject3DRenderable* _pO3DRenderable) const;

private:

  void RenderScene(CRenderManager* _pRM, CCamera* _pCamera);

  bool InitMaterialEffects(CXMLTreeNode& _treeMaterialEffects);

};

#endif

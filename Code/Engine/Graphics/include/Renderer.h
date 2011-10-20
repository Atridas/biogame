#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "base.h"

class CRenderTarget;
class CRendererStep;
class CSceneRendererStep;
//class CPreSceneRendererStep;
class CPostSceneRendererStep;
class CProcess;
class CCamera;
class CObject3DRenderable;

class CRenderer :
  public CBaseControl
{
  struct SRenderPath
  {
    set<string> m_PreSceneRenderSteps;
    set<string> m_PostSceneRenderSteps;
    set<string> m_SceneRenderers;
    bool m_bActive;
  };

public:

  CRenderer() : m_szFileName(""), m_bRenderPathsChanged(false),m_pActiveCamera(0) {};
  ~CRenderer() {Done();};

  bool Init(const string& _szFileName);
  
  CPostSceneRendererStep* GetPostSceneRendererStep(string _szName);
  
  void ActivateRenderPath  (const string& _szRenderPath);
  void DeactivateRenderPath(const string& _szRenderPath);
  void SetUniqueRenderPath(const string&  _szRenderPath);
  void GetActiveRenderPaths(set<string>& _RenderPaths) const;
  
  void     SetActiveCamera(CCamera * _pActiveCamera) { m_pActiveCamera = _pActiveCamera; };
  CCamera* GetActiveCamera() const                   { return m_pActiveCamera; };

  void Render(CProcess* _pProcess);
  void Update(float _fDeltaTime);

  CRenderTarget* GetRenderTarget(const string& _szRenderTarget) const 
  { 
    map<string,CRenderTarget*>::const_iterator l_it = m_mapRenderTargets.find(_szRenderTarget);
    if(l_it != m_mapRenderTargets.end())
    {
      return l_it->second;
    }
    else
    {
      return 0;
    }
  }

protected:
  virtual void Release();
private:

  
  void InitRenderVectors(CCamera* _pCamera,
                         vector<CObject3DRenderable*>& _vOpaqueObjects, 
                         vector<CObject3DRenderable*>& _vAlphaObjects, 
                         vector<CObject3DRenderable*>& _vParticleEmiters);

  void ActivateRenderPaths();

  string m_szFileName;

  CCamera* m_pCamera, *m_pActiveCamera;

  map<string,CRenderTarget*> m_mapRenderTargets;
  string m_szDefaultRenderTarget;
  
  map<string,CSceneRendererStep*>     m_mapSceneRendererSteps;
  map<string,CPostSceneRendererStep*> m_mapPostSceneRendererSteps;

  vector<CSceneRendererStep*>     m_vSceneRendererSteps;
  vector<CPostSceneRendererStep*> m_vPostSceneRendererSteps;

  map<string, SRenderPath*> m_mapRenderPaths;
  bool m_bRenderPathsChanged;
};

#endif


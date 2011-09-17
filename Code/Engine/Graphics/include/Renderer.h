#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "base.h"

class CRenderTarget;
class CRendererStep;
class CSceneRendererStep;
class CPreSceneRendererStep;
class CPostSceneRendererStep;
class CProcess;
class CCamera;

class CRenderer :
  public CBaseControl
{
  struct SRenderPath
  {
    set<string> m_PreSceneRenderSteps;
    set<string> m_PostSceneRenderSteps;
    string m_szSceneRenderer;
    bool m_bActive;
  };

public:

  CRenderer() : m_szFileName(""), m_pCurrentSceneRenderer(0),m_bRenderPathsChanged(false) {};
  ~CRenderer() {Done();};

  bool Init(const string& _szFileName);
  
  CPostSceneRendererStep* GetPostSceneRendererStep(string _szName);
  CPreSceneRendererStep* GetPreSceneRendererStep(string _szName);

  void SetSceneRenderer(const string& _szRendererName);
  
  void ActivateRenderPath  (const string& _szRenderPath);
  void DeactivateRenderPath(const string& _szRenderPath);

  void Render(CProcess* _pProcess);

protected:
  virtual void Release();
private:

  void ActivateRenderPaths();

  string m_szFileName;

  CCamera* m_pCamera;

  CSceneRendererStep* m_pCurrentSceneRenderer;

  map<string,CRenderTarget*> m_mapRenderTargets;
  string m_szDefaultRenderTarget;
  
  map<string,CSceneRendererStep*> m_mapSceneRendererSteps;
  map<string,CPostSceneRendererStep*> m_mapPostSceneRendererSteps;
  map<string,CPreSceneRendererStep*> m_mapPreSceneRendererSteps;

  vector<CPreSceneRendererStep*> m_vPreSceneRendererSteps;
  vector<CSceneRendererStep*> m_vSceneRendererSteps;
  vector<CPostSceneRendererStep*> m_vPostSceneRendererSteps;

  map<string, SRenderPath*> m_mapRenderPaths;
  bool m_bRenderPathsChanged;
};

#endif


#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "base.h"

class CRendererStep;
class CSceneRendererStep;
class CPreSceneRendererStep;
class CPostSceneRendererStep;
class CProcess;

class CRenderer :
  public CBaseControl
{

public:

  CRenderer() : m_szFileName(""), m_pCurrentSceneRenderer(0) {};
  ~CRenderer() {Done();};

  bool Init(const string& _szFileName);
  
  CPostSceneRendererStep* GetPostSceneRendererStep(string _szName);
  CPreSceneRendererStep* GetPreSceneRendererStep(string _szName);

  void SetSceneRenderer(const string& _szRendererName);

  void Render(CProcess* _pProcess);

protected:
  virtual void Release();
private:

  string m_szFileName;

  CSceneRendererStep* m_pCurrentSceneRenderer;

  map<string,CPostSceneRendererStep*> m_mapPostSceneRendererSteps;
  map<string,CPreSceneRendererStep*> m_mapPreSceneRendererSteps;

  vector<CPreSceneRendererStep*> m_vPreSceneRendererSteps;
  vector<CSceneRendererStep*> m_vSceneRendererSteps;
  vector<CPostSceneRendererStep*> m_vPostSceneRendererSteps;

};

#endif


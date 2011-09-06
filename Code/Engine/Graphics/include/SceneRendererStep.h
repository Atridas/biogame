#pragma once
#ifndef __SCENE_RENDERER_STEP_H__
#define __SCENE_RENDERER_STEP_H__

#include "base.h"
#include "RendererStep.h"

class CXMLTreeNode;
class CProcess;

class CSceneRendererStep :
  public CRendererStep
{

public:
  CSceneRendererStep() : m_szStaticMeshEffect(""),m_szAnimatedModelEffect(""),m_bUseMap(false) {};
  ~CSceneRendererStep() {Done();};

  bool Init(CXMLTreeNode& _treeSceneRenderer);

  virtual void Render(CProcess* _pProcess);

protected:
  virtual void Release();

  map<int,string> m_mapMaterialEffects;
  string m_szStaticMeshEffect;
  string m_szAnimatedModelEffect;
  bool m_bUseMap;

private:

  bool InitMaterialEffects(CXMLTreeNode& _treeMaterialEffects);

};

#endif

#pragma once
#ifndef __SCENE_EFFECT_MANAGER_H__
#define __SCENE_EFFECT_MANAGER_H__

#include "base.h"
//Forward declarations-------------
class CXMLTreeNode;
class CSceneEffect;
class CRenderManager;
class CProc;
//---------------------------------


class CSceneEffectManager :
  public CBaseControl
{

public:
  vector<CSceneEffect *> m_PreRenderSceneEffects;
  vector<CSceneEffect *> m_RenderSceneEffects;
  vector<CSceneEffect *> m_CaptureFrameBufferSceneEffects;
  vector<CSceneEffect *> m_CaptureFrameBufferSceneEffectsAfterPostRender;
  vector<CSceneEffect *> m_PostRenderSceneEffects;

public:
  CSceneEffectManager();
  ~CSceneEffectManager(){Done();};

  void Release();
  void LoadXML(const std::string &FileName);
  void PreRender(CRenderManager *RM, CProc *Proc);
  void ActivateRenderSceneEffects();
  void CaptureFrameBuffers(CRenderManager *RM);
  void PostRender(CRenderManager *RM);
  void CaptureFrameBuffersAfterPostRender(CRenderManager *RM);
};
#endif
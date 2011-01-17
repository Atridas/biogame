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


class CSceneEffectManager
{

public:
  std::vector<CSceneEffect *> m_PreRenderSceneEffects;
  std::vector<CSceneEffect *> m_RenderSceneEffects;
  std::vector<CSceneEffect *> m_CaptureFrameBufferSceneEffects;
  std::vector<CSceneEffect *> m_CaptureFrameBufferSceneEffectsAfterPostRender;
  std::vector<CSceneEffect *> m_PostRenderSceneEffects;

public:
  CSceneEffectManager();
  ~CSceneEffectManager();

  void Destroy();
  void LoadXML(const std::string &FileName);
  void PreRender(CRenderManager *RM, CProc *Proc);
  void ActivateRenderSceneEffects();
  void CaptureFrameBuffers(CRenderManager *RM);
  void PostRender(CRenderManager *RM);
  void CaptureFrameBuffersAfterPostRender(CRenderManager *RM);
};
#endif
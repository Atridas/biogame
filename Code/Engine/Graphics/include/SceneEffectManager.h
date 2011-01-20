#pragma once
#ifndef __SCENE_EFFECT_MANAGER_H__
#define __SCENE_EFFECT_MANAGER_H__

#include "base.h"
//Forward declarations-------------
class CXMLTreeNode;
class CSceneEffect;
class CRenderManager;
class CProcess;
//---------------------------------


class CSceneEffectManager :
  public CBaseControl
{

public:/*private:?*/

  vector<CSceneEffect*> m_vPreRenderSceneEffects;
  vector<CSceneEffect*> m_vRenderSceneEffects;
  vector<CSceneEffect*> m_vCaptureFrameBufferSceneEffects;
  vector<CSceneEffect*> m_vCaptureFrameBufferSceneEffectsAfterPostRender;
  vector<CSceneEffect*> m_vPostRenderSceneEffects;
  bool Load();
  string m_szFileName;

public:
  CSceneEffectManager();
  ~CSceneEffectManager(){Done();};

  bool Init(const CXMLTreeNode& _xmlSceneEffect);
  void Release();
  bool Load(const string& m_szFileName);
  void PreRender(CRenderManager* _pRM, CProcess* _pProc);
  void ActivateRenderSceneEffects();
  void CaptureFrameBuffers(CRenderManager* _pRM);
  void PostRender(CRenderManager* _pRM);
  void CaptureFrameBuffersAfterPostRender(CRenderManager* _pRM);
};

#endif
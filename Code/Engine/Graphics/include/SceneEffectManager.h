#pragma once
#ifndef __SCENE_EFFECT_MANAGER_H__
#define __SCENE_EFFECT_MANAGER_H__

#include "base.h"
#include "Utils/MapManager.h"
#include "SceneEffect.h"
//Forward declarations-------------
class CXMLTreeNode;
class CShadowMapRenderToTexture;
class CLight;
class CRenderManager;
class CProcess;
//---------------------------------


class CSceneEffectManager :
  public CMapManager<CSceneEffect>
{

public:

  CSceneEffectManager();
  ~CSceneEffectManager(){Done();};

  void Release();
  bool Load(const string& m_szFileName);
  void PreRender(CRenderManager* _pRM, CProcess* _pProc);
  void ActivateRenderSceneEffects();
  void CaptureFrameBuffers(CRenderManager* _pRM);
  void PostRender(CRenderManager* _pRM);
  void GUIRender(CRenderManager* _pRM);
  void WarpRender(CRenderManager* _pRM);
  void CaptureFrameBuffersAfterPostRender(CRenderManager* _pRM);

  void SetShadowMapLightCast(CLight* _pLight);

private:

  vector<CSceneEffect*> m_vPreRenderSceneEffects;
  vector<CSceneEffect*> m_vRenderSceneEffects;
  vector<CSceneEffect*> m_vCaptureFrameBufferSceneEffects;
  vector<CSceneEffect*> m_vCaptureFrameBufferSceneEffectsAfterPostRender;
  vector<CSceneEffect*> m_vPostRenderSceneEffects;
  vector<CSceneEffect*> m_vGUISceneEffects;
  vector<CSceneEffect*> m_vWarpSceneEffects;

  bool Load();
  string m_szFileName;

  CShadowMapRenderToTexture* m_pShadowMapEffect;
};

#endif
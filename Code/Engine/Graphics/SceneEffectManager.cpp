#include "SceneEffectManager.h"


CSceneEffectManager::CSceneEffectManager(void):
  m_PreRenderSceneEffects(0),
  m_RenderSceneEffects(0),
  m_CaptureFrameBufferSceneEffects(0),
  m_CaptureFrameBufferSceneEffectsAfterPostRender(0),
  m_PostRenderSceneEffects(0)
{
  SetOk(true);
}


void CSceneEffectManager::Release()
{

}
  
void CSceneEffectManager::LoadXML(const string &_szFileName)
{
  //StaticMeshManager

}
  
void CSceneEffectManager::PreRender(CRenderManager *RM, CProc *Proc)
{

}
  
void CSceneEffectManager::ActivateRenderSceneEffects()
{

}
  
void CSceneEffectManager::CaptureFrameBuffers(CRenderManager *RM)
{

}
  
void CSceneEffectManager::PostRender(CRenderManager *RM)
{

}
  
void CSceneEffectManager::CaptureFrameBuffersAfterPostRender(CRenderManager *RM)
{

}


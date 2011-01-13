#include "EffectManager.h"
#include "EffectTechnique.h"
#include "Effect.h"


void CEffectManager::ActivateCamera(const Mat44f& _mViewMatrix, const Mat44f& _mProjectionMatrix, const Vect3f& _vCameraEye)
{

}

void CEffectManager::Load(const string& _szFileName)
{

}

void CEffectManager::Reload()
{

}

CEffect* CEffectManager::GetEffect(const string& _szName)
{
  return 0;
}

CEffectTechnique* CEffectManager::GetEffectTechnique(const string& _szName)
{
  return 0;
}

void CEffectManager::Release()
{
  //map
  m_DefaultTechniqueEffectMap.clear();
  //mapmanager
  m_Effects.Release();

  //pointers
  CHECKED_DELETE(m_pStaticMeshTechnique);
  CHECKED_DELETE(m_pAnimatedModelTechnique);
}
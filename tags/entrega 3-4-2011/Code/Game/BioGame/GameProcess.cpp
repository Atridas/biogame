#include "GameProcess.h"
#include "RenderManager.h"
#include "ThPSCamera.h"
#include "InputManager.h"
#include "RenderableObjectsManager.h"

#include "Core.h"

void CGameProcess::Update(float _fElapsedTime)
{
  float l_fPitch, l_fYaw;

  Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();

  l_fPitch = m_pRiggleObject3D->GetPitch();
  l_fYaw = m_pRiggleObject3D->GetYaw();
  
  m_pRiggleObject3D->SetYaw(l_fYaw-l_vVec.x*_fElapsedTime);
  //m_pObjectBot->SetYaw(m_pObject->GetYaw()-FLOAT_PI_VALUE/2.0f);

  l_fPitch -= l_vVec.y*_fElapsedTime;
  if(l_fPitch < - FLOAT_PI_VALUE/3) l_fPitch = - FLOAT_PI_VALUE/3;
  if(l_fPitch >   FLOAT_PI_VALUE/3) l_fPitch =   FLOAT_PI_VALUE/3;
  m_pRiggleObject3D->SetPitch(l_fPitch);
}

void CGameProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);
}

bool CGameProcess::Init()
{
  SetOk(true);

  m_pRiggleObject3D = new CObject3D;
  
  m_pObjectCamera = new CThPSCamera(
    0.1f,
    100.0f,
    35.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pRiggleObject3D,
    4.5f);

  m_pCamera = m_pObjectCamera;

  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  return IsOk();
}

void CGameProcess::Release()
{
	// ----
  CHECKED_DELETE(m_pObjectCamera);
  CHECKED_DELETE(m_pRiggleObject3D);
}

bool CGameProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _szAction)
{
	return false;
}

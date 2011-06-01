#include "Component3rdPSCamera.h"
#include "ComponentObject3D.h"
#include "ThPSCamera.h"
#include "RenderManager.h"
#include "Core.h"

bool CComponent3rdPSCamera::Init(CGameEntity *_pEntity,
            float _fCameraHeight, float _fCameraRight)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  
  m_fCameraHeight = _fCameraHeight;
  m_fCameraRight  = _fCameraRight;

  Vect3f l_vPosition = m_pObject3D->GetPosition();
  float  l_fYaw      = m_pObject3D->GetYaw();

  l_vPosition.x += m_fCameraRight * sin(l_fYaw);
  l_vPosition.y += m_fCameraHeight;
  l_vPosition.z -= m_fCameraRight * cos(l_fYaw);

  m_CameraObject.SetPosition(l_vPosition);

  //TODO més parametres? jujujujuju
  m_pCamera = new CThPSCamera(
                      0.1f,
                      100.0f,
                      35.0f * FLOAT_PI_VALUE/180.0f,
                      ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
                      &m_CameraObject,
                      4.5f);

  SetOk(true);
  return IsOk();
}

void CComponent3rdPSCamera::Release()
{
  CHECKED_DELETE(m_pCamera);
}

void CComponent3rdPSCamera::PostUpdate(float _fDeltaTime)
{
  Vect3f l_vPos = m_pObject3D->GetPosition();
  float  l_fYaw      = m_pObject3D->GetYaw();

  l_vPos.x += m_fCameraRight * sin(l_fYaw);
  l_vPos.y += m_fCameraHeight;
  l_vPos.z -= m_fCameraRight * cos(l_fYaw);

  m_CameraObject.SetPosition( l_vPos );
  m_CameraObject.SetPitch( m_pObject3D->GetPitch() );
  m_CameraObject.SetYaw  ( l_fYaw );
}


CCamera* CComponent3rdPSCamera::GetCamera() const 
{
  return m_pCamera;
};

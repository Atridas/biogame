#include "Component3rdPSCamera.h"
#include "ComponentObject3D.h"
#include "ThPSCamera.h"
#include "RenderManager.h"
#include "Core.h"

bool CComponent3rdPSCamera::Init(CGameEntity *_pEntity,
            float _fCameraHeight)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = dynamic_cast<CComponentObject3D*>(_pEntity->GetComponent(ECT_OBJECT_3D));
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats


  m_fCameraHeight = _fCameraHeight;

  Vect3f l_vPosition = m_pObject3D->GetPosition();
  l_vPosition.x += m_fCameraHeight;

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

void CComponent3rdPSCamera::Update(float _fDeltaTime)
{

  Vect3f l_vPos = m_pObject3D->GetPosition();
  l_vPos.y += m_fCameraHeight;
  m_CameraObject.SetPosition( l_vPos );
  m_CameraObject.SetPitch( m_pObject3D->GetPitch() );
  m_CameraObject.SetYaw( m_pObject3D->GetYaw() );
}


CCamera* CComponent3rdPSCamera::GetCamera() const 
{
  return m_pCamera;
};

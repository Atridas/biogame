#include "Component3rdPSCamera.h"
#include "ComponentObject3D.h"
#include "ShoulderCamera.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Core.h"

bool CComponent3rdPSCamera::Init(CGameEntity *_pEntity,
            float _fCameraHeight, float _fCameraRight, float _fZoom)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error m�s elavorats

  m_fCameraHeight = _fCameraHeight;
  m_fCameraRight  = _fCameraRight;
  m_fZoom         = _fZoom;

  m_pCamera = new CShoulderCamera(
                      0.1f,
                      100.0f,
                      55.0f * FLOAT_PI_VALUE/180.0f,
                      ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
                      &m_CameraObject,
                      m_fZoom,m_fCameraRight,m_fCameraHeight);

  SetOk(true);
  return IsOk();
}

void CComponent3rdPSCamera::Release()
{
  CHECKED_DELETE(m_pCamera);
}

void CComponent3rdPSCamera::PostUpdate(float _fDeltaTime)
{
  m_CameraObject.SetPosition( m_pObject3D->GetPosition() );
  m_CameraObject.SetPitch( m_pObject3D->GetPitch() );
  m_CameraObject.SetYaw  (  m_pObject3D->GetYaw() );

  SCollisionInfo l_CInfo;

  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();

  CPhysicUserData* l_pUserDataSHOOT = 0;
  
  l_pUserDataSHOOT = l_pPM->RaycastClosestActor(m_CameraObject.GetPosition()+Vect3f(0.0f,m_fCameraHeight,0.0f),(m_pCamera->GetLookAt()-(m_CameraObject.GetPosition()+Vect3f(0.0f,m_fCameraHeight,0.0f))).Normalize(),l_pPM->GetCollisionMask(ECG_CAMERA),l_CInfo);

  if(l_pUserDataSHOOT && l_CInfo.m_fDistance - 0.25f < m_fCameraRight)
  {
    m_pCamera->SetShoulderDistance(l_CInfo.m_fDistance - 0.25f);
  }else{
    m_pCamera->SetShoulderDistance(m_fCameraRight);
  }

  l_pUserDataSHOOT = 0;

	l_pUserDataSHOOT = l_pPM->RaycastClosestActor(m_pCamera->GetLookAt(),-m_pCamera->GetDirection().Normalize(),l_pPM->GetCollisionMask(ECG_CAMERA),l_CInfo);

  if(l_pUserDataSHOOT && l_CInfo.m_fDistance - 0.25f < m_fZoom)
  {
    m_pCamera->SetZoom(l_CInfo.m_fDistance - 0.25f);
  }else{
    m_pCamera->SetZoom(m_fZoom);
  }
  
}


CCamera* CComponent3rdPSCamera::GetCamera() const 
{
  return m_pCamera;
};

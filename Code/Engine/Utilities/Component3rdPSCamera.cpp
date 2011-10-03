#include "Component3rdPSCamera.h"
#include "ComponentObject3D.h"
#include "ShoulderCamera.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Core.h"


CComponent3rdPSCamera* CComponent3rdPSCamera::AddToEntity(CGameEntity *_pEntity,
                                        float _fCameraHeight,
                                        float _fCameraRight,
                                        float _fZoom)
{
  CComponent3rdPSCamera *l_pComp = new CComponent3rdPSCamera();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fCameraHeight, _fCameraRight, _fZoom))
  {
    l_pComp->SetEntity(_pEntity);
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponent3rdPSCamera::Init(CGameEntity *_pEntity,
            float _fCameraHeight, float _fCameraRight, float _fZoom)
{

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  m_fCameraHeight = _fCameraHeight;
  m_fCameraRight  = _fCameraRight;
  m_fZoom         = _fZoom;
  m_fTargetFOV = 55.0f * FLOAT_PI_VALUE/180.0f;

  m_pCamera = new CShoulderCamera(
                            0.1f,
                            100.0f,
                            m_fTargetFOV,
                            ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
                            &m_CameraObject,
                            m_fZoom,m_fCameraRight,m_fCameraHeight);

  m_tTimeConstantRightDistance = 6.8f;
  m_tTimeConstantObjectDistance = 8.7f;
  m_fTimeConstantFOV = 5.0f;

  SetOk(true);
  return IsOk();
}

void CComponent3rdPSCamera::Release()
{
  CHECKED_DELETE(m_pCamera);
}

void CComponent3rdPSCamera::PostUpdate(float _fDeltaTime)
{
  float l_fPrevTargetRightDistance = m_fTargetRightDistance;
  float l_fPrevTargetObjectDistance = m_fTargetObjectDistance;

  m_CameraObject.SetPosition( m_pObject3D->GetPosition() );
  m_CameraObject.SetPitch( m_pObject3D->GetPitch() );
  m_CameraObject.SetYaw  (  m_pObject3D->GetYaw() );

  SCollisionInfo l_CInfo;

  CPhysicsManager* l_pPM = CORE->GetPhysicsManager();

  CPhysicUserData* l_pUserDataSHOOT = 0;
  
  l_pUserDataSHOOT = l_pPM->RaycastClosestActor(m_CameraObject.GetPosition()+Vect3f(0.0f,m_fCameraHeight,0.0f),(m_pCamera->GetLookAt()-(m_CameraObject.GetPosition()+Vect3f(0.0f,m_fCameraHeight,0.0f))).Normalize(),l_pPM->GetCollisionMask(ECG_CAMERA),l_CInfo);

  if(l_pUserDataSHOOT)
  {
    float l_fDistance = l_CInfo.m_fDistance - 0.4f;

    if(l_fDistance < 0.01f)
    {
      l_fDistance = 0.01f;
    }

    if(l_fDistance < m_fCameraRight)
    {
      m_fTargetRightDistance = l_fDistance;
   
    }else{
      m_fTargetRightDistance = m_fCameraRight;
    }
  }

  if(m_fTargetRightDistance != l_fPrevTargetRightDistance)
  {
    m_fTimeRightDistance = 0.0f;
    m_fPrevRightDistance = m_pCamera->GetShoulderDistance();
  }

  if(m_pCamera->GetShoulderDistance() != m_fTargetRightDistance)
  {
    m_fTimeRightDistance += _fDeltaTime;
    m_pCamera->SetShoulderDistance(m_fPrevRightDistance + FirstOrderSystem((m_fTargetRightDistance - m_fPrevRightDistance),m_tTimeConstantRightDistance,m_fTimeRightDistance));
  }

  l_pUserDataSHOOT = 0;

	l_pUserDataSHOOT = l_pPM->RaycastClosestActor(m_pCamera->GetLookAt(),-m_pCamera->GetDirection().Normalize(),l_pPM->GetCollisionMask(ECG_CAMERA),l_CInfo);

  if(l_pUserDataSHOOT)
  {
    float l_fDistance = l_CInfo.m_fDistance - 0.4f;

    if(l_fDistance < 0.01f)
    {
      l_fDistance = 0.01f;
    }

    if(l_fDistance < m_fZoom)
    {
      m_fTargetObjectDistance = l_fDistance;
    
    }else{
      m_fTargetObjectDistance = m_fZoom;
    }
  }else{
    m_fTargetObjectDistance = m_fZoom;
  }
  
  if(m_fTargetObjectDistance != l_fPrevTargetObjectDistance)
  {
    m_fTimeObjectDistance = 0.0f;
    m_fPrevObjectDistance = m_pCamera->GetZoom();
  }

  if(m_pCamera->GetZoom() != m_fTargetObjectDistance)
  {
    m_fTimeObjectDistance += _fDeltaTime;
    m_pCamera->SetZoom(m_fPrevObjectDistance + FirstOrderSystem(m_fTargetObjectDistance - m_fPrevObjectDistance, m_tTimeConstantObjectDistance, m_fTimeObjectDistance));
  }

  if(m_pCamera->GetFov() != m_fTargetFOV)
  {
    m_fTimeFOV += _fDeltaTime;
    m_pCamera->SetFov(m_fPrevFOV + FirstOrderSystem(m_fTargetFOV - m_fPrevFOV, m_fTimeConstantFOV, m_fTimeFOV));
  }
}

float CComponent3rdPSCamera::FirstOrderSystem(float _fInput, float _fTimeConstant, float _fTime)
{
  return _fInput*(1 - exp(-_fTimeConstant*_fTime));
}

CCamera* CComponent3rdPSCamera::GetCamera() const 
{
  return m_pCamera;
};

void CComponent3rdPSCamera::SetZoom(float _fZoom, float _tTimeConstantObjectDistance)
{
  m_fZoom = _fZoom;
  m_tTimeConstantObjectDistance = _tTimeConstantObjectDistance;
}

void CComponent3rdPSCamera::SetFOV(float _fFOV, float _tTimeConstantFOV)
{
  float l_fValue = _fFOV * FLOAT_PI_VALUE/180.0f;

  if(m_fTargetFOV != l_fValue)
  {
    m_fTimeFOV = 0.0f;
    m_fPrevFOV = m_pCamera->GetFov();
    m_fTargetFOV = l_fValue;
    m_fTimeConstantFOV = _tTimeConstantFOV;
  }
}
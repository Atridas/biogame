#include "ComponentPlayerController.h"
#include "ComponentObject3D.h"
#include "ThPSCamera.h"
#include "RenderManager.h"
#include "Core.h"
#include "InputManager.h"
#include "ActionManager.h"

bool CComponentPlayerController::Init(CGameEntity *_pEntity,
            const string& _szMoveFwd,
            const string& _szMoveBack,
            const string& _szMoveLeft,
            const string& _szMoveRight,

            const string& _szWalk,
            const string& _szRun,
  
            float _fWalkSpeed,
            float _fRunSpeed,
            float _fYawSpeed,
            float _fPitchSpeed,
  
            float _fMaxPitchAngle,
            float _fMinPitchAngle,
            
            float _fCameraHeight)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = dynamic_cast<CComponentObject3D*>(_pEntity->GetComponent(ECT_OBJECT_3D));
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats


  m_szMoveFwd   = _szMoveFwd;
  m_szMoveBack  = _szMoveBack;
  m_szMoveLeft  = _szMoveLeft;
  m_szMoveRight = _szMoveRight;
  
  m_szRun  = _szRun;
  m_szWalk = _szWalk;

  m_fSpeed = m_fWalkSpeed  = _fWalkSpeed;
  m_fRunSpeed   = _fRunSpeed;
  m_fYawSpeed   = _fYawSpeed;
  m_fPitchSpeed = _fPitchSpeed;

  m_fMaxPitchAngle = _fMaxPitchAngle;
  m_fMinPitchAngle = _fMinPitchAngle;

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

void CComponentPlayerController::Release()
{
  CHECKED_DELETE(m_pCamera);
}

void CComponentPlayerController::Update(float _fDeltaTime)
{
  assert(IsOk());

  float l_fPitch, l_fYaw;

  Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();

  l_fPitch = m_pObject3D->GetPitch();
  l_fYaw = m_pObject3D->GetYaw();
  
  l_fYaw -= l_vVec.x*_fDeltaTime*m_fYawSpeed;
  //m_pObjectBot->SetYaw(m_pObject->GetYaw()-FLOAT_PI_VALUE/2.0f);

  l_fPitch -= l_vVec.y*_fDeltaTime*m_fPitchSpeed;
  if(l_fPitch < m_fMinPitchAngle) l_fPitch = m_fMinPitchAngle;
  if(l_fPitch > m_fMaxPitchAngle) l_fPitch = m_fMaxPitchAngle;
  
  m_pObject3D->SetPitch(l_fPitch);
  m_pObject3D->SetYaw(l_fYaw);

  


  
  CActionManager* l_pActionManager = CORE->GetActionManager();

  if(l_pActionManager->IsActionActive(m_szRun))
  {
    m_fSpeed = m_fRunSpeed;
  } else if(l_pActionManager->IsActionActive(m_szWalk))
  {
    m_fSpeed = m_fWalkSpeed;
  }
  
  if(l_pActionManager->IsActionActive(m_szMoveFwd))
  {
    Vect3f l_vDirection(cos(l_fYaw), 0, sin(l_fYaw) );
    m_pObject3D->SetPosition( m_pObject3D->GetPosition() + l_vDirection * (m_fSpeed*_fDeltaTime));
  }
  if(l_pActionManager->IsActionActive(m_szMoveBack))
  {
    Vect3f l_vDirection(cos(l_fYaw), 0, sin(l_fYaw) );
    m_pObject3D->SetPosition( m_pObject3D->GetPosition() - l_vDirection * (m_fSpeed*_fDeltaTime));
  }
  if(l_pActionManager->IsActionActive(m_szMoveLeft))
  {
    Vect3f l_vLeft(cos(l_fYaw+FLOAT_PI_VALUE/2), 0, sin(l_fYaw+FLOAT_PI_VALUE/2) );
    m_pObject3D->SetPosition( m_pObject3D->GetPosition() + l_vLeft * (m_fSpeed*_fDeltaTime));
  }
  if(l_pActionManager->IsActionActive(m_szMoveRight))
  {
    Vect3f l_vLeft(cos(l_fYaw+FLOAT_PI_VALUE/2), 0, sin(l_fYaw+FLOAT_PI_VALUE/2) );
    m_pObject3D->SetPosition( m_pObject3D->GetPosition() - l_vLeft * (m_fSpeed*_fDeltaTime));
  }

  Vect3f l_vPos = m_pObject3D->GetPosition();
  l_vPos.y += m_fCameraHeight;
  m_CameraObject.SetPosition( l_vPos );
  m_CameraObject.SetPitch( m_pObject3D->GetPitch() );
  m_CameraObject.SetYaw( m_pObject3D->GetYaw() );
}


CCamera* CComponentPlayerController::GetCamera() const 
{
  return m_pCamera;
};
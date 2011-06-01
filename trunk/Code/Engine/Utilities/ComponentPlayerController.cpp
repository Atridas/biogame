#include "ComponentPlayerController.h"
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "Core.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"

bool CComponentPlayerController::Init(CGameEntity *_pEntity)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pMovement = _pEntity->GetComponent<CComponentMovement>(ECT_MOVEMENT);
  assert(m_pMovement); //TODO fer missatges d'error més elavorats

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  SetOk(true);
  return IsOk();
}

bool CComponentPlayerController::Init(CGameEntity *_pEntity,
            const string& _szMoveForward,
            const string& _szMoveBack,
            const string& _szMoveLeft,
            const string& _szMoveRight,

            const string& _szWalk,
            const string& _szRun,

            const string& _szIdleAnimation,
            const string& _szForwardAnimation,
            const string& _szBackAnimation,
            const string& _szLeftAnimation,
            const string& _szRightAnimation,
  
            float _fWalkSpeed,
            float _fRunSpeed,
            float _fYawSpeed,
            float _fPitchSpeed,
  
            float _fMaxPitchAngle,
            float _fMinPitchAngle)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pMovement = _pEntity->GetComponent<CComponentMovement>(ECT_MOVEMENT);
  assert(m_pMovement); //TODO fer missatges d'error més elavorats

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  CComponentRenderableObject *l_pComponentRO = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  assert(l_pComponentRO); //TODO fer missatges d'error més elavorats
  m_pAnimatedModel = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pComponentRO->GetRenderableObject());
  assert(m_pAnimatedModel); //TODO fer missatges d'error més elavorats
  
  m_iCurrentAnimation = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimationId(_szIdleAnimation);
  m_pAnimatedModel->GetAnimatedInstanceModel()->BlendCycle(m_iCurrentAnimation,0.f);

  m_szMoveForward   = _szMoveForward;
  m_szMoveBack      = _szMoveBack;
  m_szMoveLeft      = _szMoveLeft;
  m_szMoveRight     = _szMoveRight;
  
  m_szRun  = _szRun;
  m_szWalk = _szWalk;

  m_szIdleAnimation    = _szIdleAnimation;
  m_szForwardAnimation = _szForwardAnimation;
  m_szBackAnimation    = _szBackAnimation;
  m_szLeftAnimation    = _szLeftAnimation;
  m_szRightAnimation   = _szRightAnimation;

  m_fSpeed = m_fWalkSpeed  = _fWalkSpeed;
  m_fRunSpeed   = _fRunSpeed;
  m_fYawSpeed   = _fYawSpeed;
  m_fPitchSpeed = _fPitchSpeed;

  m_fMaxPitchAngle = _fMaxPitchAngle;
  m_fMinPitchAngle = _fMinPitchAngle;

  SetOk(true);
  return IsOk();
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

  CAnimatedInstanceModel *l_pAnimatedInstanceModel = m_pAnimatedModel->GetAnimatedInstanceModel();
  int l_iDesiredAnim = l_pAnimatedInstanceModel->GetAnimationId(m_szIdleAnimation);
  
  if(l_pActionManager->IsActionActive(m_szMoveForward))
  {
    Vect3f l_vDirection(cos(l_fYaw), 0, sin(l_fYaw) );
    m_pMovement->m_vMovement += l_vDirection * (m_fSpeed*_fDeltaTime);

    l_iDesiredAnim = l_pAnimatedInstanceModel->GetAnimationId(m_szForwardAnimation);
  }
  if(l_pActionManager->IsActionActive(m_szMoveBack))
  {
    Vect3f l_vDirection(cos(l_fYaw), 0, sin(l_fYaw) );
    m_pMovement->m_vMovement -= l_vDirection * (m_fSpeed*_fDeltaTime);

    l_iDesiredAnim = l_pAnimatedInstanceModel->GetAnimationId(m_szBackAnimation);
  }
  if(l_pActionManager->IsActionActive(m_szMoveLeft))
  {
    Vect3f l_vLeft(cos(l_fYaw+FLOAT_PI_VALUE/2), 0, sin(l_fYaw+FLOAT_PI_VALUE/2) );
    m_pMovement->m_vMovement += l_vLeft * (m_fSpeed*_fDeltaTime);

    l_iDesiredAnim = l_pAnimatedInstanceModel->GetAnimationId(m_szLeftAnimation);
  }
  if(l_pActionManager->IsActionActive(m_szMoveRight))
  {
    Vect3f l_vLeft(cos(l_fYaw+FLOAT_PI_VALUE/2), 0, sin(l_fYaw+FLOAT_PI_VALUE/2) );
    m_pMovement->m_vMovement -= l_vLeft * (m_fSpeed*_fDeltaTime);

    l_iDesiredAnim = l_pAnimatedInstanceModel->GetAnimationId(m_szRightAnimation);
  }

  if(l_iDesiredAnim != m_iCurrentAnimation)
  {
    l_pAnimatedInstanceModel->ClearCycle(0.3f);
    l_pAnimatedInstanceModel->BlendCycle(l_iDesiredAnim, 0.3f);
    m_iCurrentAnimation = l_iDesiredAnim;
  }
}

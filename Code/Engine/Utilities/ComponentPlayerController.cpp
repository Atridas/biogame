#include "ComponentPlayerController.h"
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "Core.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "Component3rdPSCamera.h"
#include "Camera.h"
#include "AnimatedInstanceModel.h"
#include "cal3d/cal3d.h"
#include "ComponentLaser.h"
#include "ComponentRenderableObject.h"
#include "ComponentRagdoll.h"

#include "PhysicsManager.h"

bool CComponentPlayerController::Init(CGameEntity *_pEntity)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pMovement = _pEntity->GetComponent<CComponentMovement>(ECT_MOVEMENT);
  assert(m_pMovement); //TODO fer missatges d'error m�s elavorats

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error m�s elavorats

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

            const string& _szAim,
            const string& _szShoot,

            const string& _szIdleAnimation,
            const string& _szForwardAnimation,
            const string& _szBackAnimation,
            const string& _szLeftAnimation,
            const string& _szRightAnimation,
            const string& _szAimAnimation,
            const string& _szShootAnimation,
  
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
  assert(m_pMovement); //TODO fer missatges d'error m�s elavorats

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error m�s elavorats

  CComponentRenderableObject *l_pComponentRO = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  assert(l_pComponentRO); //TODO fer missatges d'error m�s elavorats
  m_pAnimatedModel = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pComponentRO->GetRenderableObject());
  assert(m_pAnimatedModel); //TODO fer missatges d'error m�s elavorats
  
  //m_iCurrentAnimation = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimationId(_szIdleAnimation);
  //m_pAnimatedModel->GetAnimatedInstanceModel()->BlendCycle(m_iCurrentAnimation,0.f);

  m_szMoveForward   = _szMoveForward;
  m_szMoveBack      = _szMoveBack;
  m_szMoveLeft      = _szMoveLeft;
  m_szMoveRight     = _szMoveRight;
  
  m_szRun  = _szRun;
  m_szWalk = _szWalk;

  m_szAim = _szAim;
  m_szShoot = _szShoot;

  m_szIdleAnimation    = _szIdleAnimation;
  m_szForwardAnimation = _szForwardAnimation;
  m_szBackAnimation    = _szBackAnimation;
  m_szLeftAnimation    = _szLeftAnimation;
  m_szRightAnimation   = _szRightAnimation;
  m_szAimAnimation     = _szAimAnimation;
  m_szShootAnimation   = _szShootAnimation;

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

  l_fPitch -= l_vVec.y*_fDeltaTime*m_fPitchSpeed;
  if(l_fPitch < m_fMinPitchAngle) l_fPitch = m_fMinPitchAngle;
  if(l_fPitch > m_fMaxPitchAngle) l_fPitch = m_fMaxPitchAngle;
  
  //m_pObject3D->SetPitch(l_fPitch);
  //m_pObject3D->SetYaw(l_fYaw);

  
  CActionManager* l_pActionManager = CORE->GetActionManager();

  if(l_pActionManager->IsActionActive(m_szRun))
  {
    m_fSpeed = m_fRunSpeed;
  } else if(l_pActionManager->IsActionActive(m_szWalk))
  {
    m_fSpeed = m_fWalkSpeed;
  }
}


void CComponentPlayerController::Shoot()
{

  CAnimatedInstanceModel *l_pAnimatedInstanceModel = m_pAnimatedModel->GetAnimatedInstanceModel();


  int l_iAnim = l_pAnimatedInstanceModel->GetAnimationId(m_szShootAnimation);
  l_pAnimatedInstanceModel->ExecuteAction(l_iAnim,0.2f);

  CCamera* l_pCamera = GetEntity()->GetComponent<CComponent3rdPSCamera>(ECT_3RD_PERSON_SHOOTER_CAMERA)->GetCamera();
  //Vect3f l_vPos = m_pObject3D->GetPosition();
  Vect3f l_vPos = l_pCamera->GetEye();
  Vect3f l_vDir = l_pCamera->GetDirection().Normalize();

  l_vPos += l_vDir;

  SCollisionInfo l_CInfo;
  CPhysicUserData* l_pUserData = 0;

  CPhysicsManager *l_pPM = CORE->GetPhysicsManager();

  l_pUserData = l_pPM->RaycastClosestActor(l_vPos,l_vDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT),l_CInfo);

  if( l_pUserData )
  {
    Vect3f l_vCenterPoint = l_CInfo.m_CollisionPoint;

    CalSkeleton *l_pSkeleton = l_pAnimatedInstanceModel->GetAnimatedCalModel()->getSkeleton();
    CalCoreSkeleton *l_pCoreSkeleton = l_pSkeleton->getCoreSkeleton();
    CalBone* l_pBone = l_pSkeleton->getBone( l_pCoreSkeleton->getCoreBoneId("Bip01 R Hand") );

    CalVector l_vTanslationBone = l_pBone->getTranslationAbsolute();

    Mat44f l_mat = GetEntity()->GetComponent<CComponentRenderableObject>()->GetRenderableObject()->GetMat44();
    //Mat44f l_mat = m_pObject3D->GetMat44();

    Vect4f l_vTanslationBone2(-l_vTanslationBone.x, l_vTanslationBone.y, l_vTanslationBone.z, 1);

    l_vTanslationBone2 = l_mat * l_vTanslationBone2;


    CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();
    (new CComponentLaser())->Init(l_pLaser,
                                  Vect3f(l_vTanslationBone2.x,l_vTanslationBone2.y,l_vTanslationBone2.z),
                                  l_vCenterPoint,
                                  1.f);

    if(l_pUserData->GetEntity())
    {
      SEvent l_impacte;
      l_impacte.Msg = SEvent::REBRE_IMPACTE;
      l_impacte.Info[0].Type = SEventInfo::FLOAT;
      l_impacte.Info[0].f    = 20.f;
      l_impacte.Receiver = l_pUserData->GetEntity()->GetGUID();
      l_impacte.Sender = GetEntity()->GetGUID();

      //TODO usar un manager
      CORE->GetEntityManager()->SendEvent(l_impacte);
      //l_pUserData->GetEntity()->ReceiveEvent(l_impacte);
    }
  }
  else
  {
      
    Vect3f l_vCenterPoint = l_vPos + l_vDir * 1000;

    CalSkeleton *l_pSkeleton = l_pAnimatedInstanceModel->GetAnimatedCalModel()->getSkeleton();
    CalCoreSkeleton *l_pCoreSkeleton = l_pSkeleton->getCoreSkeleton();
    CalBone* l_pBone = l_pSkeleton->getBone( l_pCoreSkeleton->getCoreBoneId("Bip01 R Hand") );

    CalVector l_vTanslationBone = l_pBone->getTranslationAbsolute();

    Mat44f l_mat = GetEntity()->GetComponent<CComponentRenderableObject>()->GetRenderableObject()->GetMat44();
    //Mat44f l_mat = m_pObject3D->GetMat44();

    Vect4f l_vTanslationBone2(-l_vTanslationBone.x, l_vTanslationBone.y, l_vTanslationBone.z, 1);

    l_vTanslationBone2 = l_mat * l_vTanslationBone2;


    CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();
    (new CComponentLaser())->Init(l_pLaser,
                                  Vect3f(l_vTanslationBone2.x,l_vTanslationBone2.y,l_vTanslationBone2.z),
                                  l_vCenterPoint,
                                  1.f);
  }
}

void CComponentPlayerController::Die()
{
  CComponentRagdoll *l_pRC = GetEntity()->GetComponent<CComponentRagdoll>();
  if(!l_pRC)
  {
    l_pRC = new CComponentRagdoll();
    if(!l_pRC->Init(GetEntity(), "Data/Animated Models/Riggle/Skeleton.xml"))
    {
      delete l_pRC;
      return;
    }
  }
  l_pRC->SetActive(true);
}

void CComponentPlayerController::Respawn()
{
  CComponentRagdoll *l_pRC = GetEntity()->GetComponent<CComponentRagdoll>();
  if(l_pRC)
  {
    l_pRC->SetActive(false);
  }
}

bool CComponentPlayerController::Cover()
{
  SCollisionInfo l_sCInfo;
  CPhysicUserData* l_pUserData = CheckCover(l_sCInfo);

  m_vCoverNormal = Vect3f(0.0f);
  m_vCoverPosition = Vect3f(0.0f);

  if(l_pUserData && l_sCInfo.m_fDistance <= 2.0f)
  {
    m_vCoverNormal = l_sCInfo.m_Normal;
    m_vCoverPosition = l_sCInfo.m_CollisionPoint;
    return true;
  }

  return false;
}

CPhysicUserData* CComponentPlayerController::CheckCover(SCollisionInfo& _sCInfo)
{  
  CCamera* l_pCamera = GetEntity()->GetComponent<CComponent3rdPSCamera>(ECT_3RD_PERSON_SHOOTER_CAMERA)->GetCamera();
  Vect3f l_vPos = m_pObject3D->GetPosition() - Vect3f(0.0f,0.5f,0.0f);

  Mat33f l_mRot;
  l_mRot.SetIdentity();
  l_mRot.RotByAngleY(-m_pObject3D->GetYaw());

  Vect3f l_vDir = l_mRot*Vect3f(1.0f,0.0f,0.0f);

  CPhysicsManager *l_pPM = CORE->GetPhysicsManager();

  CPhysicUserData* l_pUserData = l_pPM->RaycastClosestActor(l_vPos,l_vDir,l_pPM->GetCollisionMask(ECG_COBERTURES),_sCInfo);

  return l_pUserData;
}
#include "ComponentPlayerController.h"
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "Core.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "SceneEffectManager.h"
#include "DrawQuadSceneEffect.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "Component3rdPSCamera.h"
#include "Camera.h"
#include "AnimatedInstanceModel.h"
#include "cal3d/cal3d.h"
#include "ComponentLaser.h"
#include "ComponentRenderableObject.h"
#include "ComponentRagdoll.h"
#include "ComponentCover.h"
#include "ComponentVida.h"
#include "ComponentParticleShootPlayer.h"

#include "PhysicsManager.h"
#include "AnimatedCoreModel.h"
#include "Material.h"

#define BLOOD_FADEOUT_TIME 2.5

CComponentPlayerController* CComponentPlayerController::AddToEntity(CGameEntity *_pEntity)
{
  CComponentPlayerController *l_pComp = new CComponentPlayerController();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity))
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

bool CComponentPlayerController::Init(CGameEntity *_pEntity)
{
  m_pMovement = _pEntity->GetComponent<CComponentMovement>(ECT_MOVEMENT);
  assert(m_pMovement); //TODO fer missatges d'error més elavorats

  m_pObject3D = _pEntity->GetComponent<CComponentObject3D>(ECT_OBJECT_3D);
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  CComponentRenderableObject *l_pComponentRO = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);
  assert(l_pComponentRO); //TODO fer missatges d'error més elavorats
  m_pAnimatedModel = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pComponentRO->GetRenderableObject());
  assert(m_pAnimatedModel); //TODO fer missatges d'error més elavorats

 

  m_vPickUps.clear();

  SetOk(true);
  return IsOk();
}

void CComponentPlayerController::Update(float _fDeltaTime)
{
  assert(IsOk());

  CComponentVida* l_pComponentVida = GetEntity()->GetComponent<CComponentVida>();
  const vector<CMaterial*>& l_vMaterials = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();

  CDrawQuadSceneEffect* l_pDamage = (CDrawQuadSceneEffect*)CORE->GetSceneEffectManager()->GetResource("damage");
  CDrawQuadSceneEffect* l_pBlood = (CDrawQuadSceneEffect*)CORE->GetSceneEffectManager()->GetResource("blood_decal");

  float l_fHP = l_pComponentVida->GetHP();
  float l_fMaxHP = l_pComponentVida->GetMaxHP();

  float l_fMin = 0.3f;
  float l_fMax = 1.5f;
  float l_fGlowIntensity = l_fHP/l_fMaxHP * (l_fMax - l_fMin) + l_fMin;

  vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

  while(l_itMaterial != l_vMaterials.end())
  {
    (*l_itMaterial)->SetGlowIntensity(l_fGlowIntensity);
    ++l_itMaterial;
  }

  if(l_pDamage)
  {
    if(l_fHP < l_fMaxHP)
    {
      float l_fDamageAlpha = 1.0f - (l_fHP/l_fMaxHP);
      l_fDamageAlpha = l_fDamageAlpha + abs(l_fDamageAlpha*0.8f*sinf(5.0f*m_fBloodTime));
      if(l_fDamageAlpha > 1.0f)
      {
        l_fDamageAlpha = 1.0f;
      }

      l_pDamage->SetActive(true);
      l_pDamage->SetAlphaFactor(l_fDamageAlpha);
    }else{
      l_pDamage->SetActive(false);
    }
  }

  if(m_fBloodFadeOutTime > 0.0f)
  {
    l_pBlood->SetAlphaFactor(m_fBloodFadeOutTime/(float)BLOOD_FADEOUT_TIME);
    l_pBlood->SetActive(true);

    m_fBloodFadeOutTime -= _fDeltaTime;

  }else{
    l_pBlood->SetActive(false);
    m_fBloodFadeOutTime = 0.0f;
  }

  
  m_fBloodTime += _fDeltaTime;

  if(m_iNumUpdates < 3)
    m_iNumUpdates++;
  if(m_iNumUpdates == 2)
  {
    CComponentRagdoll::AddToEntity(GetEntity(), "Data/Animated Models/Riggle/Skeleton.xml", ECG_RAGDOLL);
  }
}


void CComponentPlayerController::Shoot()
{

  CAnimatedInstanceModel *l_pAnimatedInstanceModel = m_pAnimatedModel->GetAnimatedInstanceModel();


  //int l_iAnim = l_pAnimatedInstanceModel->GetAnimationId(m_szShootAnimation);
  //l_pAnimatedInstanceModel->ExecuteAction(l_iAnim,0.2f);

  CCamera* l_pCamera = GetEntity()->GetComponent<CComponent3rdPSCamera>(ECT_3RD_PERSON_SHOOTER_CAMERA)->GetCamera();
  //Vect3f l_vPos = m_pObject3D->GetPosition();
  Vect3f l_vPos = l_pCamera->GetEye();
  Vect3f l_vDir = l_pCamera->GetDirection().Normalize();

  l_vPos += l_vDir;

  SCollisionInfo l_CInfo;
  CPhysicUserData* l_pUserData = 0;

  CPhysicsManager *l_pPM = PHYSICS_MANAGER;

  l_pUserData = l_pPM->RaycastClosestActorShoot(l_vPos,l_vDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT),l_CInfo,20.0f);

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
    CComponentLaser::AddToEntity(l_pLaser,
                                  Vect3f(l_vTanslationBone2.x,l_vTanslationBone2.y,l_vTanslationBone2.z),
                                  l_vCenterPoint,
                                  1.f);

    CGameEntity * l_pParticleShoot = CORE->GetEntityManager()->CreateEntity();
    CComponentParticleShootPlayer::AddToEntity(l_pParticleShoot,
                                                Vect3f(l_vTanslationBone2.x,l_vTanslationBone2.y,l_vTanslationBone2.z),
                                                l_vCenterPoint);
    
    if(l_pUserData->GetEntity())
    {
      SEvent l_impacte;
      l_impacte.Msg = SEvent::REBRE_IMPACTE;
      l_impacte.Info[0].Type = SEventInfo::FLOAT;
      l_impacte.Info[0].f    = 20.f;
      l_impacte.Receiver = l_pUserData->GetEntity()->GetGUID();
      l_impacte.Sender = GetEntity()->GetGUID();

      ENTITY_MANAGER->SendEvent(l_impacte);
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
    CComponentLaser::AddToEntity(l_pLaser,
                                  Vect3f(l_vTanslationBone2.x,l_vTanslationBone2.y,l_vTanslationBone2.z),
                                  l_vCenterPoint,
                                  1.f);

    CGameEntity * l_pParticleShoot = CORE->GetEntityManager()->CreateEntity();
    CComponentParticleShootPlayer::AddToEntity(l_pParticleShoot,
                                                Vect3f(l_vTanslationBone2.x,l_vTanslationBone2.y,l_vTanslationBone2.z),
                                                l_vCenterPoint);
  }
}

void CComponentPlayerController::Use()
{
  CEntityManager* l_pEM = ENTITY_MANAGER;
  CPhysicsManager *l_pPM = PHYSICS_MANAGER;
  vector<CPhysicUserData*> l_vImpactObjects;

  //missatge
  SEvent l_Missatge;
  l_Missatge.Msg = SEvent::INTERACCIO;
  l_Missatge.Sender = GetEntity()->GetGUID();


  //esfera d'us
  l_pPM->OverlapSphereActor( 5.0f                               //radiusSphere
                            ,m_pObject3D->GetCenterPosition()   //posSphere
                            ,l_vImpactObjects                   // impactObjects
                            );

  //TODO usar un manager

  vector<CPhysicUserData*>::iterator l_itInit = l_vImpactObjects.begin();
  vector<CPhysicUserData*>::iterator l_itEnd  = l_vImpactObjects.end();
  vector<CPhysicUserData*>::iterator l_itIt;
  
  for(l_itIt = l_itInit; l_itIt < l_itEnd; ++l_itIt)
  {
    CGameEntity* l_pEntity = (*l_itIt)->GetEntity();
    if(l_pEntity)
    {
      l_Missatge.Receiver = l_pEntity->GetGUID();
      l_pEM->SendEvent(l_Missatge);
    }
  }
}

void CComponentPlayerController::Die()
{
  CComponentRagdoll *l_pRC = GetEntity()->GetComponent<CComponentRagdoll>();
  if(l_pRC)
  {
    //l_pRC->SetActive(true);
  }
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
    m_pCoverEntity = l_pUserData->GetEntity();

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


bool CComponentPlayerController::HasPickUp(const string& _szPickUp)
{
  vector<string>::iterator l_vIt;

  for(l_vIt = m_vPickUps.begin(); l_vIt != m_vPickUps.end(); ++l_vIt)
  {
    if((*l_vIt) == _szPickUp)
      return true;
  }

  return false;
}

void CComponentPlayerController::AddPickUp(const string& _szPickUp)
{
  m_vPickUps.push_back(_szPickUp);
}

bool CComponentPlayerController::RemovePickUp(const string& _szPickUp)
{
  vector<string>::iterator l_vIt;

  for(l_vIt = m_vPickUps.begin(); l_vIt != m_vPickUps.end(); ++l_vIt)
  {
    if((*l_vIt) == _szPickUp)
    {
      m_vPickUps.erase(l_vIt);
      return true;
    }
  }

  return false;
}

void CComponentPlayerController::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Receiver == GetEntity()->GetGUID())
  {
    if(_Event.Msg == SEvent::PICKUP)
    {
      if(_Event.Info[0].Type == SEventInfo::STRING)
        AddPickUp(_Event.Info[0].str);
    }else if(_Event.Msg == SEvent::REBRE_IMPACTE)
    {
      m_fBloodFadeOutTime = BLOOD_FADEOUT_TIME;
      m_fBloodTime = 0.0f;
    }

  }
}
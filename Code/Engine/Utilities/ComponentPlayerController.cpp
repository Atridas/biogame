#include "ComponentPlayerController.h"
#include "ComponentObject3D.h"
#include "ComponentMovement.h"
#include "Core.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "Renderer.h"
#include "PostSceneRendererStep.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "Component3rdPSCamera.h"
#include "ComponentArma.h"
#include "Camera.h"
#include "AnimatedInstanceModel.h"
#include "cal3d/cal3d.h"
#include "ComponentLaser.h"
#include "ComponentRenderableObject.h"
#include "ComponentRagdoll.h"
#include "ComponentCover.h"
#include "ComponentVida.h"

#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "AnimatedCoreModel.h"
#include "Material.h"

#define BLOOD_FADEOUT_TIME 2.5f
#define SHOCK_WAVE_VELOCITY 2.5f
#define DAMAGE_FORCE 100.0f
#define SHOOT_POWER 30.0f

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

void CComponentPlayerController::UpdatePostPhysX(float _fDeltaTime)
{
  assert(IsOk());

  CComponentVida* l_pComponentVida = GetEntity()->GetComponent<CComponentVida>();
  const vector<CMaterial*>& l_vMaterials = m_pAnimatedModel->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();

  CPostSceneRendererStep* l_pDamage = CORE->GetRenderer()->GetPostSceneRendererStep("damage_gui");
  CPostSceneRendererStep* l_pBlood = CORE->GetRenderer()->GetPostSceneRendererStep("blood_gui");

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
      l_pDamage->SetAlpha(l_fDamageAlpha);
    }else{
      l_pDamage->SetActive(false);
    }
  }

  if(m_fBloodFadeOutTime > 0.0f)
  {
    l_pBlood->SetAlpha(m_fBloodFadeOutTime/(float)BLOOD_FADEOUT_TIME);
    l_pBlood->SetActive(true);

    m_fBloodFadeOutTime -= _fDeltaTime;

  }else{
    l_pBlood->SetActive(false);
    m_fBloodFadeOutTime = 0.0f;
  }

  m_fBloodTime += _fDeltaTime;


  CPostSceneRendererStep* l_pShockWave = CORE->GetRenderer()->GetPostSceneRendererStep("shock_wave");
  //CPostSceneRendererStep* l_pCaptureFrameBuffers = (CDrawQuadSceneEffect*)CORE->GetSceneEffectManager()->GetResource("capture_frame_buffer_scene_effect_with_post_fx");

  
  if(l_pShockWave->IsActive() == true)
  {
    m_fForceTime += _fDeltaTime;

    l_pShockWave->SetTime(m_fForceTime*SHOCK_WAVE_VELOCITY);

    if(m_fForceTime > 3.0f)
    {
      l_pShockWave->SetActive(false);
      //l_pCaptureFrameBuffers->SetActive(false);
    }
  }

  //if(m_iNumUpdates < 3)
  //  m_iNumUpdates++;
  //if(m_iNumUpdates == 2)
  //{
  //  CComponentRagdoll::AddToEntity(GetEntity(), "Data/Animated Models/Riggle/Skeleton.xml", ECG_RAGDOLL_PLAYER);
  //}
}


void CComponentPlayerController::Shoot()
{
  CEntityManager* l_pEM = ENTITY_MANAGER;
  CGameEntity* l_pPlayerEntity = GetEntity();

  CAnimatedInstanceModel *l_pAnimatedInstanceModel = m_pAnimatedModel->GetAnimatedInstanceModel();

  CCamera* l_pCamera = GetEntity()->GetComponent<CComponent3rdPSCamera>(ECT_3RD_PERSON_SHOOTER_CAMERA)->GetCamera();
  CComponentArma* l_pArma = GetEntity()->GetComponent<CComponentArma>();

  Vect3f l_vPosArma = l_pArma->GetPosition();
  Vect3f l_vDirArma = l_pArma->GetAimDirection();
  Vect3f l_vPos = l_pCamera->GetEye();
  Vect3f l_vDir = l_pCamera->GetDirection().Normalize();

  SCollisionInfo l_CInfo;
  CPhysicUserData* l_pUserData = 0;

  l_vPosArma -= l_vDirArma*0.1f;
  
  l_pEM->InitParticles("disparar", l_vPosArma + l_vDirArma*0.2f, Vect3f(.25f,.5f,.25f), 2.5f, l_vDir);

  CPhysicsManager *l_pPM = PHYSICS_MANAGER;

  l_pUserData = l_pPM->RaycastClosestActor(l_vPos,l_vDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT_PLAYER),l_CInfo);

  if(l_pUserData && l_pUserData->GetEntity() != l_pPlayerEntity)
  {
    Vect3f l_vCenterPoint = l_CInfo.m_CollisionPoint;

    l_vDir = (l_vCenterPoint-l_vPosArma).Normalize();

    l_pUserData = l_pPM->RaycastClosestActor(l_vPosArma,l_vDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT_PLAYER),l_CInfo);

    if(l_pUserData)
    {

      if(l_pUserData->GetEntity() != l_pPlayerEntity)
      {
        l_pEM->InitLaser(l_vPosArma,l_vDir,20.f, l_pPM->GetCollisionMask(ECG_RAY_SHOOT_PLAYER));
        //SEvent l_impacte;
        //l_impacte.Msg = SEvent::REBRE_IMPACTE;
        //l_impacte.Info[0].Type = SEventInfo::FLOAT;
        //l_impacte.Info[0].f    = 20.f;
        //l_impacte.Receiver = l_pUserData->GetEntity()->GetGUID();
        //l_impacte.Sender = l_pPlayerEntity->GetGUID();
        //l_impacte.Info[1].Type = SEventInfo::VECTOR;
        //l_impacte.Info[1].v.x = l_vDir.x;
        //l_impacte.Info[1].v.y = l_vDir.y;
        //l_impacte.Info[1].v.z = l_vDir.z;
        //l_impacte.Info[2].Type = SEventInfo::PTR;
        //l_impacte.Info[2].ptr = (void*)(l_pUserData->GetActor());
        //l_impacte.Info[3].Type = SEventInfo::VECTOR;
        //l_impacte.Info[3].v.x = l_CInfo.m_CollisionPoint.x;
        //l_impacte.Info[3].v.y = l_CInfo.m_CollisionPoint.y;
        //l_impacte.Info[3].v.z = l_CInfo.m_CollisionPoint.z;
        //ENTITY_MANAGER->SendEvent(l_impacte);
      }
    }

  }else{

    Vect3f l_vPuntLlunya = l_vPos+100.f*l_vDir;

    l_vDir = ((l_vPos+100.f*l_vDir)-l_vPosArma).Normalize();

    l_pUserData = l_pPM->RaycastClosestActor(l_vPosArma,l_vDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT_PLAYER),l_CInfo);

    if(l_pUserData)
    {

      if(l_pUserData->GetEntity() != l_pPlayerEntity)
      {
        l_pEM->InitLaser(l_vPosArma,l_vDir,20.f, l_pPM->GetCollisionMask(ECG_RAY_SHOOT_PLAYER));
        //SEvent l_impacte;
        //l_impacte.Msg = SEvent::REBRE_IMPACTE;
        //l_impacte.Info[0].Type = SEventInfo::FLOAT;
        //l_impacte.Info[0].f    = 20.f;
        //l_impacte.Receiver = l_pUserData->GetEntity()->GetGUID();
        //l_impacte.Sender = l_pPlayerEntity->GetGUID();
        //l_impacte.Info[1].Type = SEventInfo::VECTOR;
        //l_impacte.Info[1].v.x = l_vDir.x;
        //l_impacte.Info[1].v.y = l_vDir.y;
        //l_impacte.Info[1].v.z = l_vDir.z;
        //l_impacte.Info[2].Type = SEventInfo::PTR;
        //l_impacte.Info[2].ptr = (void*)(l_pUserData->GetActor());
        //l_impacte.Info[3].Type = SEventInfo::VECTOR;
        //l_impacte.Info[3].v.x = l_CInfo.m_CollisionPoint.x;
        //l_impacte.Info[3].v.y = l_CInfo.m_CollisionPoint.y;
        //l_impacte.Info[3].v.z = l_CInfo.m_CollisionPoint.z;
        //ENTITY_MANAGER->SendEvent(l_impacte);
      }

    }else{
      l_pEM->InitLaser(l_vPosArma,l_vDir,20.f, l_pPM->GetCollisionMask(ECG_RAY_SHOOT_PLAYER));
    }
  }

}

void CComponentPlayerController::Force()
{
  CGameEntity* l_pPlayerEntity = GetEntity();

  assert(l_pPlayerEntity);

  CComponentObject3D* l_pObject3d = l_pPlayerEntity->GetComponent<CComponentObject3D>();

  assert(l_pObject3d);

  CPostSceneRendererStep* l_pShockWave = CORE->GetRenderer()->GetPostSceneRendererStep("shock_wave");
  //CDrawQuadSceneEffect* l_pCaptureFrameBuffers = (CDrawQuadSceneEffect*)CORE->GetSceneEffectManager()->GetResource("capture_frame_buffer_scene_effect_with_post_fx");

  assert(l_pShockWave);
  //assert(l_pCaptureFrameBuffers);

  //if(l_pShockWave->IsActive() == false)
  //{
    //l_pCaptureFrameBuffers->SetActive(true);
    l_pShockWave->SetActive(true);
    m_fForceTime = 0.0f;

    Vect3f l_vPos = l_pObject3d->GetPosition();
    vector<CPhysicUserData*> l_vImpactObjects;
    CPhysicsManager *l_pPM = PHYSICS_MANAGER;

    Mat33f l_mRot;
    l_mRot.SetIdentity();
    l_mRot.RotByAngleY(-m_pObject3D->GetYaw());

    Vect3f l_vDir = l_mRot*Vect3f(1.0f,0.0f,0.0f);

    l_pPM->OverlapSphereActor(2.0f,l_vPos+3.0f*l_vDir,l_vImpactObjects,l_pPM->GetCollisionMask(ECG_FORCE));

    vector<CPhysicUserData*>::iterator l_itUserData;
    vector<CPhysicUserData*>::iterator l_itUserDataEnd = l_vImpactObjects.end();

    set<CGameEntity*> l_vImpactEntities;

    for(l_itUserData = l_vImpactObjects.begin(); l_itUserData != l_itUserDataEnd; ++l_itUserData)
    {
      CPhysicUserData* l_pUserData = *l_itUserData;
      l_vImpactEntities.insert(l_pUserData->GetEntity());
    }

    set<CGameEntity*>::iterator l_itEntity;
    set<CGameEntity*>::iterator l_itEntityEnd = l_vImpactEntities.end();

    for(l_itEntity = l_vImpactEntities.begin(); l_itEntity != l_itEntityEnd; ++l_itEntity)
    {
      CGameEntity* l_pEntity = *l_itEntity;

      if(l_pPlayerEntity != l_pEntity)
      {
        SEvent l_impacte;
        l_impacte.Msg = SEvent::REBRE_FORCE;
        l_impacte.Info[0].Type = SEventInfo::FLOAT;
        l_impacte.Info[0].f    = DAMAGE_FORCE;
        l_impacte.Receiver = l_pEntity->GetGUID();
        l_impacte.Sender = l_pPlayerEntity->GetGUID();

        ENTITY_MANAGER->SendEvent(l_impacte);
      }
    }

  //}

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
  l_pPM->OverlapSphereActor( 5.0f                                  //radiusSphere
                            ,m_pObject3D->GetCenterPosition()      //posSphere
                            ,l_vImpactObjects,                     //impactObjects
                            l_pPM->GetCollisionMask(ECG_ESCENARI));//collision_mask

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
    l_pRC->ApplyPhysics(true);
  }
}

void CComponentPlayerController::Respawn()
{
  CComponentRagdoll *l_pRC = GetEntity()->GetComponent<CComponentRagdoll>();
  if(l_pRC)
  {
    l_pRC->ApplyPhysics(false);
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

      assert(_Event.Info[3].Type == SEventInfo::VECTOR);
      Vect3f l_vPos(_Event.Info[3].v.x, _Event.Info[3].v.y, _Event.Info[3].v.z);
    
      ENTITY_MANAGER->InitParticles("impacte ragdoll", l_vPos, Vect3f(.5f,.5f,.5f), 5.f);
  
    }

  }
}
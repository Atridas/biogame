#include "ComponentIABrain.h"
#include "Core.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "PhysicsManager.h"
#include "ComponentObject3D.h"
#include "ComponentLaser.h"
#include "ComponentRagdoll.h" 
#include "ComponentNavNode.h"
#include "ComponentVida.h"
#include "IAManager.h"
#include "GraphDefines.h"
#include "PhysxBone.h"
#include "PhysicActor.h"
#include "cal3d\cal3d.h"
//#include "ComponentParticleShootMiner.h"

#define SHOOT_POWER 20.0f

CComponentIABrain* CComponentIABrain::AddToEntity(CGameEntity *_pEntity, const string& _szPlayerEntityName, const string& _szRagdollName)
{
  CComponentIABrain *l_pComp = new CComponentIABrain();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szPlayerEntityName, _szRagdollName))
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

bool CComponentIABrain::Init(CGameEntity* _pEntity, const string& _szPlayerEntityName, const string& _szRagdollName)
{
  m_szRagdollName = _szRagdollName;

  m_pPlayer = CORE->GetEntityManager()->GetEntity(_szPlayerEntityName);

  SetOk(true);
  return IsOk();
}


void CComponentIABrain::Shoot()
{

  CComponentRenderableObject* l_pCR = GetEntity()->GetComponent<CComponentRenderableObject>();
  CRenderableAnimatedInstanceModel* l_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCR->GetRenderableObject());
  CAnimatedInstanceModel *l_pAnimatedInstanceModel = l_pRAIM->GetAnimatedInstanceModel();

  // ---------------------------------------------------------------------------------------------------------
  Vect3f l_vPlayerPos = m_pPlayer->GetComponent<CComponentObject3D>()->GetPosition();

  l_vPlayerPos += Vect3f(0, .5f, 0);
  // ---------------------------------------------------------------------------------------------------------
  CalSkeleton *l_pSkeleton = l_pAnimatedInstanceModel->GetAnimatedCalModel()->getSkeleton();
  CalCoreSkeleton *l_pCoreSkeleton = l_pSkeleton->getCoreSkeleton();
  CalBone* l_pBone = l_pSkeleton->getBone( l_pCoreSkeleton->getCoreBoneId("Bip01 R Hand") );

  CalVector l_vTanslationBone = l_pBone->getTranslationAbsolute();

  Mat44f l_mat = l_pCR->GetRenderableObject()->GetMat44();
  //Mat44f l_mat = m_pObject3D->GetMat44();

  Vect4f l_vAux(-l_vTanslationBone.x, l_vTanslationBone.y, l_vTanslationBone.z, 1);

  l_vAux = l_mat * l_vAux;

  Vect3f l_vMyHand(l_vAux.x / l_vAux.w, l_vAux.y / l_vAux.w, l_vAux.z / l_vAux.w);

  // ------------------------------------------------------------------------------------------------------
  Vect3f l_vPos = l_vMyHand;//l_pPlayerPos + Vect3f(0, .75f, 0);
  Vect3f l_vDir = (l_vPlayerPos - l_vMyHand).GetNormalized();
  
  CEntityManager* l_pEM = ENTITY_MANAGER;
  l_pEM->InitParticles("disparar", l_vPos, Vect3f(.5f,.5f,.5f), 2.5f, l_vDir);
  l_pEM->InitLaser(l_vPos,l_vDir,20.f, CORE->GetPhysicsManager()->GetCollisionMask(ECG_RAY_SHOOT));

  //SCollisionInfo l_CInfo;
  //CPhysicUserData* l_pUserData = 0;
  //
  //CPhysicsManager *l_pPM = CORE->GetPhysicsManager();
  //
  //
  //l_pUserData = l_pPM->RaycastClosestActor(l_vPos,l_vDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT),l_CInfo);
  //
  //if( l_pUserData )
  //{
  //  Vect3f l_vCenterPoint = l_CInfo.m_CollisionPoint;
  //
  //  //CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();
  //  //CComponentLaser::AddToEntity(l_pLaser,
  //  //                              Vect3f(l_vMyHand.x,l_vMyHand.y,l_vMyHand.z),
  //  //                              l_vCenterPoint,
  //  //                              1.f);
  //  
  //  l_pEM->InitLaser(l_vPos,l_vDir,20.f);
  //  if(l_pUserData->GetEntity())
  //  {
  //    l_pEM->InitLaser(l_vPos,l_vDir,20.f);
  //
  //    //SEvent l_impacte;
  //    //l_impacte.Msg = SEvent::REBRE_IMPACTE;
  //    //l_impacte.Info[0].Type = SEventInfo::FLOAT;
  //    //l_impacte.Info[0].f    = 20.f;
  //    //l_impacte.Info[3].Type = SEventInfo::VECTOR;
  //    //l_impacte.Info[3].v.x = l_CInfo.m_CollisionPoint.x;
  //    //l_impacte.Info[3].v.y = l_CInfo.m_CollisionPoint.y;
  //    //l_impacte.Info[3].v.z = l_CInfo.m_CollisionPoint.z;
  //    //l_impacte.Receiver = l_pUserData->GetEntity()->GetGUID();
  //    //l_impacte.Sender = GetEntity()->GetGUID();
  //    //
  //    //CORE->GetEntityManager()->SendEvent(l_impacte);
  //  }
  //}
  //else
  //{
  //  l_pEM->InitLaser(l_vPos,l_vDir,20.f);
  //  //Vect3f l_vCenterPoint = l_vMyHand + l_vDir * 100;
  //
  //  //CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();
  //  //CComponentLaser::AddToEntity(l_pLaser,
  //  //                            l_vMyHand,
  //  //                            l_vCenterPoint,
  //  //                            1.f);
  //}
}

void CComponentIABrain::ReceiveShoot(SEvent _sEvent)
{
  CPhysxBone* l_pHeadBone = 0;
  CPhysicActor* l_pActor = 0;
  CComponentRagdoll* l_pRagdoll = 0;

  if(_sEvent.Msg == SEvent::REBRE_IMPACTE)
  {
    l_pRagdoll = GetEntity()->GetComponent<CComponentRagdoll>();

    assert(l_pRagdoll);

    float l_fVida = GetEntity()->GetComponent<CComponentVida>()->GetHP();

    if(l_fVida > 0.0f)
    {

      l_pHeadBone = l_pRagdoll->GetBone("Bip01 Head");

      assert(l_pHeadBone);

      if(_sEvent.Info[2].Type == SEventInfo::PTR)
      {
        l_pActor = (CPhysicActor*)_sEvent.Info[2].ptr;
      }else{
        LOGGER->AddNewLog(ELL_ERROR,"CComponentIABrain::ReciveShoot El missatge a Info[2] no es del tipus PTR");
        return;
      }

      if(l_pHeadBone->GetPhysxActor() == l_pActor)
      {
        Die();

        Vect3f l_vDir(_sEvent.Info[1].v.x,_sEvent.Info[1].v.y,_sEvent.Info[1].v.z);

        l_pActor->AddForceAtLocalPos(l_vDir,Vect3f(0.0f),SHOOT_POWER);
      }

    }else{

      l_pActor = (CPhysicActor*)_sEvent.Info[2].i;

      Vect3f l_vDir(_sEvent.Info[1].v.x,_sEvent.Info[1].v.y,_sEvent.Info[1].v.z);

      l_pActor->AddForceAtLocalPos(l_vDir,Vect3f(0.0f),SHOOT_POWER);
    }
  }


}

void CComponentIABrain::ReceiveForce(SEvent _sEvent)
{
  Mat44f l_matBonePos;
  CPhysxBone* l_pPhysxBone = 0;
  CComponentRagdoll* l_pRagdoll = 0;
  Vect3f l_vSenderPos;
  Vect3f l_vDirection;

  if(_sEvent.Msg == SEvent::REBRE_FORCE)
  {
    Die();

    l_pRagdoll = GetEntity()->GetComponent<CComponentRagdoll>();

    if(l_pRagdoll)
    {
      l_vSenderPos = ENTITY_MANAGER->GetEntity(_sEvent.Sender)->GetComponent<CComponentObject3D>()->GetPosition();

      l_pPhysxBone = l_pRagdoll->GetBone("Bip01 Spine");
      assert(l_pPhysxBone);
      l_pPhysxBone->GetPhysxActor()->GetMat44(l_matBonePos);
      l_vDirection = (l_matBonePos.GetPos() - l_vSenderPos).Normalize();
      l_pPhysxBone->GetPhysxActor()->AddForceAtLocalPos(l_vDirection,Vect3f(0.0f),50.0f);

      l_pPhysxBone = l_pRagdoll->GetBone("Bip01 Spine1");
      assert(l_pPhysxBone);
      l_pPhysxBone->GetPhysxActor()->GetMat44(l_matBonePos);
      l_vDirection = (l_matBonePos.GetPos() - l_vSenderPos).Normalize();
      l_pPhysxBone->GetPhysxActor()->AddForceAtLocalPos(l_vDirection,Vect3f(0.0f),50.0f);

      l_pPhysxBone = l_pRagdoll->GetBone("Bip01 Spine2");
      assert(l_pPhysxBone);
      l_pPhysxBone->GetPhysxActor()->GetMat44(l_matBonePos);
      l_vDirection = (l_matBonePos.GetPos() - l_vSenderPos).Normalize();
      l_pPhysxBone->GetPhysxActor()->AddForceAtLocalPos(l_vDirection,Vect3f(0.0f),50.0f);
    }
  }
}

void CComponentIABrain::Update(float _fDeltaTime)
{
  //if(m_iNumUpdates < 3)
  //  m_iNumUpdates++;
  //if(m_iNumUpdates == 2)
  //{
  //  CComponentRagdoll::AddToEntity(GetEntity(), m_szRagdollName, ECG_RAGDOLL);
  //}
}

void CComponentIABrain::Die()
{
  GetEntity()->DeleteComponent(CBaseComponent::ECT_PHYSX_CONTROLLER);

  CComponentRagdoll *l_pRC = GetEntity()->GetComponent<CComponentRagdoll>();

  if(l_pRC)
  {
    l_pRC->SetActive(true);
  }
  
  SEvent l_morir;
  l_morir.Msg = SEvent::MORIR;
  l_morir.Receiver = l_morir.Sender = GetEntity()->GetGUID();
      
  ENTITY_MANAGER->SendEvent(l_morir);
}

void CComponentIABrain::PlanPathToCobertura()
{
  m_PathToCobertura = CORE->GetIAManager()->GetClosestCobertura(GetEntity()->GetComponent<CComponentObject3D>()->GetPosition());
  
  if( !m_PathToCobertura.empty() )
  {
    vector<CGraphNode*>::iterator first = m_PathToCobertura.begin();
    vector<CGraphNode*>::iterator last  = m_PathToCobertura.end();

    while ((first!=last)&&(first!=--last))
      swap (*first++,*last);
  }
}

Vect3f CComponentIABrain::GetNextNodePosition() const
{
  if(!m_PathToCobertura.empty())
    return m_PathToCobertura[m_PathToCobertura.size() - 1]->GetPosition();
  else
    return GetEntity()->GetComponent<CComponentObject3D>()->GetPosition();
}

bool CComponentIABrain::ArrivedAtDestination() const
{
  return m_PathToCobertura.size() == 1;
}

bool CComponentIABrain::ArrivedAtNode(float _fDistanceSq) const
{
  Vect3f l_vPos = GetEntity()->GetComponent<CComponentObject3D>()->GetPosition();
  Vect3f l_vNodePos = GetNextNodePosition();

  if(l_vPos.SqDistance(l_vNodePos) < _fDistanceSq)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void CComponentIABrain::SetNextNode()
{
  if(!m_PathToCobertura.empty())
    m_PathToCobertura.pop_back();
}

void CComponentIABrain::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::REBRE_IMPACTE)
  {
    ReceiveShoot(_Event);
  }else if(_Event.Msg == SEvent::REBRE_FORCE)
  {
    ReceiveForce(_Event);
  }
}
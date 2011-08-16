#include "ComponentIABrain.h"
#include "Core.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentRenderableObject.h"
#include "PhysicsManager.h"
#include "ComponentObject3D.h"
#include "ComponentLaser.h"
#include "ComponentRagdoll.h" 
#include "ComponentNavNode.h"
#include "IAManager.h"
#include "GraphDefines.h"
#include "cal3d\cal3d.h"



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

  //l_vPos += l_vDir;

  SCollisionInfo l_CInfo;
  CPhysicUserData* l_pUserData = 0;

  CPhysicsManager *l_pPM = CORE->GetPhysicsManager();

  l_pUserData = l_pPM->RaycastClosestActor(l_vPos,l_vDir,l_pPM->GetCollisionMask(ECG_RAY_SHOOT),l_CInfo);

  if( l_pUserData )
  {
    Vect3f l_vCenterPoint = l_CInfo.m_CollisionPoint;

    CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();
    CComponentLaser::AddToEntity(l_pLaser,
                                  Vect3f(l_vMyHand.x,l_vMyHand.y,l_vMyHand.z),
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

      CORE->GetEntityManager()->SendEvent(l_impacte);
    }
  }
  else
  {
    Vect3f l_vCenterPoint = l_vMyHand + l_vDir * 100;

    CGameEntity * l_pLaser = CORE->GetEntityManager()->CreateEntity();
    CComponentLaser::AddToEntity(l_pLaser,
                                l_vMyHand,
                                l_vCenterPoint,
                                1.f);
  }
}

void CComponentIABrain::Update(float _fDeltaTime)
{
  if(m_iNumUpdates < 3)
    m_iNumUpdates++;
  if(m_iNumUpdates == 2)
  {
    CComponentRagdoll::AddToEntity(GetEntity(), m_szRagdollName, ECG_RAGDOLL);
  }
}

void CComponentIABrain::Die()
{
  GetEntity()->DeleteComponent(CBaseComponent::ECT_PHYSX_CONTROLLER);

  CComponentRagdoll *l_pRC = GetEntity()->GetComponent<CComponentRagdoll>();

  if(l_pRC)
  {
    l_pRC->SetActive(true);
  }
  
  
}

void CComponentIABrain::PlanPathToCobertura()
{
  m_PathToCobertura = CORE->GetIAManager()->GetClosestCobertura(GetEntity()->GetComponent<CComponentObject3D>()->GetPosition());
  
  vector<CGraphNode*>::iterator first = m_PathToCobertura.begin();
  vector<CGraphNode*>::iterator last  = m_PathToCobertura.end();
  while ((first!=last)&&(first!=--last))
    swap (*first++,*last);
}

Vect3f CComponentIABrain::GetNextNodePosition() const
{
  return m_PathToCobertura[m_PathToCobertura.size() - 1]->GetPosition();
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
  m_PathToCobertura.pop_back();
}
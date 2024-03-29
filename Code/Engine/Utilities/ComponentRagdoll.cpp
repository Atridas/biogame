#include "ComponentRagdoll.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentObject3D.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "ComponentPhysXController.h"
#include "EntityManager.h"
#include "Core.h"
#include "ComponentMovement.h"
#include "ComponentObject3D.h"
#include "ComponentAnimation.h"

CComponentRagdoll* CComponentRagdoll::AddToEntity(CGameEntity *_pEntity, const string& _szSkeletonFile, int _iCollisionGroup)
{
  CComponentRagdoll *l_pComp = new CComponentRagdoll();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szSkeletonFile, _iCollisionGroup))
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

bool CComponentRagdoll::Init(CGameEntity* _pEntity, const string& _szSkeletonFile, int _iCollisionGroup)
{
  CComponentObject3D* l_pO3D = _pEntity->GetComponent<CComponentObject3D>();
  CComponentRenderableObject* l_pCRO = _pEntity->GetComponent<CComponentRenderableObject>();

  assert(l_pO3D);
  assert(l_pCRO);

  m_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCRO->GetRenderableObject());

  Mat44f l_mat44 = _pEntity->GetComponent<CComponentObject3D>()->GetMat44();

  m_pRAIM->SetPosition(l_mat44.GetPos());

  m_pRagdoll = new CPhysxSkeleton(false);
  CalModel* l_pCalModel = m_pRAIM->GetAnimatedInstanceModel()->GetAnimatedCalModel();
  bool l_bOk = m_pRagdoll->Init(_szSkeletonFile,l_pCalModel,l_pO3D->GetMat44(),_iCollisionGroup, _pEntity);

  SetOk(l_bOk);
  return IsOk();
}

void CComponentRagdoll::Release()
{
  CHECKED_DELETE(m_pRagdoll);
}

void CComponentRagdoll::ApplyPhysics(bool _bValue)
{
  CComponentAnimation* l_pAnimation = GetEntity()->GetComponent<CComponentAnimation>();

  if(_bValue)
  {
    if(l_pAnimation)
      l_pAnimation->SetActive(false);

    m_pRagdoll->SetRagdollActive(true);

  }else{

    CComponentObject3D* l_pO3d = GetEntity()->GetComponent<CComponentObject3D>();
    Mat44f l_matTransform;

    l_matTransform.SetIdentity();

    l_matTransform.SetPos(l_pO3d->GetPosition());

    m_pRAIM->SetMat44(l_matTransform);

    m_pRagdoll->SetRagdollActive(false);

    if(l_pAnimation)
      l_pAnimation->SetActive(true);
  }
}

void CComponentRagdoll::Enable()
{
  m_pRagdoll->SetCollisions(true);
  m_pRagdoll->SetTransformAfterUpdate(m_pRAIM->GetMat44());
}

void CComponentRagdoll::Disable()
{
  m_pRagdoll->SetCollisions(false);
  m_pRagdoll->SetRagdollActive(false);

  Mat44f l_matTransform = m_pRagdoll->GetTransform();
  l_matTransform.Translate(l_matTransform.GetPos() - Vect3f(0.0f,1000.0f,0.0f));

  m_pRagdoll->SetTransformAfterUpdate(l_matTransform);
}

CPhysxBone* CComponentRagdoll::GetBone(const string& _szBoneName)
{
  return m_pRagdoll->GetPhysxBoneByName(_szBoneName);
}

Vect3f CComponentRagdoll::GetPosition()
{
  if(m_pRagdoll)
  {
    Mat44f m = m_pRagdoll->GetTransform();
    return m.GetPos();
  }

  return Vect3f(0.0f);
}

void CComponentRagdoll::UpdatePrePhysX(float _fDeltaTime)
{
  if(!m_pRagdoll->IsRagdollActive())
  {
    CComponentMovement* l_pMovement = GetEntity()->GetComponent<CComponentMovement>();
    CComponentObject3D* l_pObject3D = GetEntity()->GetComponent<CComponentObject3D>();

    Mat44f l_matTransformMovement;
    Mat44f l_matTransform = m_pRAIM->GetMat44();

    l_matTransformMovement.SetIdentity();
    l_matTransformMovement.RotByAngleY(l_pObject3D->GetYaw() + FLOAT_PI_VALUE/2.0f);
    l_matTransformMovement.Translate(l_pMovement->m_vMovement + l_matTransform.GetPos());

    m_pRagdoll->SetTransform(l_matTransformMovement);
    
    m_pRagdoll->Update();
  }

}

void CComponentRagdoll::PostUpdate(float _fDeltaTime)
{
  if(m_pRagdoll->IsRagdollActive())
  {
    m_pRagdoll->Update();

    Mat44f m = m_pRagdoll->GetTransform();
    m_pRAIM->SetMat44(m);

    CBoundingBox l_BB = m_pRagdoll->ComputeBoundingBox();
    CBoundingSphere l_BS;
    l_BS.Init(l_BB.GetMin(), l_BB.GetMax());

    m_pRAIM->GetBoundingSphere()->Init(l_BB.GetMiddlePoint() - m.GetTranslationVector(), l_BS.GetRadius());

  }else{
    m_pRagdoll->SetTransformAfterUpdate(m_pRAIM->GetMat44());
  }
  
}

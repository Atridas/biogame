#include "ComponentRagdoll.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"
#include "ComponentObject3D.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "ComponentPhysXController.h"


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
  CComponentRenderableObject* l_pCRO = _pEntity->GetComponent<CComponentRenderableObject>();

  m_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCRO->GetRenderableObject());

  Mat44f l_mat44 = _pEntity->GetComponent<CComponentObject3D>()->GetMat44();

  m_pRagdoll = new CPhysxSkeleton(false);
  CalModel* l_pCalModel = m_pRAIM->GetAnimatedInstanceModel()->GetAnimatedCalModel();
  bool l_bOk = m_pRagdoll->Init(_szSkeletonFile,l_pCalModel,l_mat44/*m_pRAIM->GetMat44()*/,_iCollisionGroup/*ECG_OBJECTES_DINAMICS*/, _pEntity);

  SetOk(l_bOk);
  return IsOk();
}

void CComponentRagdoll::Release()
{
  CHECKED_DELETE(m_pRagdoll);
}


void CComponentRagdoll::SetActive(bool _bActive)
{
  if(m_bActive != _bActive)
  {
    m_bActive = _bActive;

    CComponentRenderableObject* l_pCRO = GetEntity()->GetComponent<CComponentRenderableObject>();
    if(m_bActive)
    {
      l_pCRO->m_bActive = false;
      m_pRagdoll->SetRagdollActive(true);
    }
    else
    {
      CComponentObject3D* l_pO3d = GetEntity()->GetComponent<CComponentObject3D>();
      Mat44f l_matTransform;

      l_matTransform.SetIdentity();

      l_matTransform.SetPos(l_pO3d->GetPosition());

      m_pRAIM->SetMat44(l_matTransform);
      
      l_pCRO->m_bActive = true;
      m_pRagdoll->SetRagdollActive(false);
    }
  }
}

CPhysxBone* CComponentRagdoll::GetBone(const string& _szBoneName)
{
  return m_pRagdoll->GetPhysxBoneByName(_szBoneName);
}

void CComponentRagdoll::PostUpdate(float _fDeltaTime)
{
  m_pRagdoll->Update();

  if(m_bActive)
  {
    //m_pRagdoll->Update();
    Mat44f m = m_pRagdoll->GetTransform();
    m_pRAIM->SetMat44(m);

    //CPhysxBone *l_pPBone = m_pRagdoll->GetPhysxBoneByName("Bip01 Spine");
    
    //Mat44f m2;
    //l_pPBone->GetPhysxActor()->GetMat44(m2);
    //Vect3f v = m2.GetTranslationVector();
    //
    //float radius = m_pRAIM->GetBoundingSphere()->GetRadius();
    //m_pRAIM->GetBoundingSphere()->Init(v - m.GetTranslationVector(), radius);

    CBoundingBox l_BB = m_pRagdoll->ComputeBoundingBox();
    CBoundingSphere l_BS;
    l_BS.Init(l_BB.GetMin(), l_BB.GetMax());
    //m_pRAIM->GetBoundingBox()->Init( l_BB.GetMax(), l_BB.GetMin());
    m_pRAIM->GetBoundingSphere()->Init(l_BB.GetMiddlePoint() - m.GetTranslationVector(), l_BS.GetRadius());

    //Això funciona però encara no se sap pq.
  }
  else
  {
    //m_pRagdoll->Update();
    Mat44f m;
    m_pRAIM->GetMat44(m);
    m_pRagdoll->SetTransform(m);
  }
}

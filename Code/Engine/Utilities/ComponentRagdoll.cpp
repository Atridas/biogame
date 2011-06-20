#include "ComponentRagdoll.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"

bool CComponentRagdoll::Init(CGameEntity* _pEntity, const string& _szSkeletonFile)
{
  assert(_pEntity && _pEntity->IsOk());
  SetEntity(_pEntity);

  CComponentRenderableObject* l_pCRO = _pEntity->GetComponent<CComponentRenderableObject>();

  m_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCRO->GetRenderableObject());

  m_pRagdoll = new CPhysxSkeleton(false);
  CalModel* l_pCalModel = m_pRAIM->GetAnimatedInstanceModel()->GetAnimatedCalModel();
  bool l_bOk = m_pRagdoll->Init(_szSkeletonFile,l_pCalModel,m_pRAIM->GetMat44(),ECG_OBJECTES_DINAMICS, _pEntity);

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
      l_pCRO->m_bActive = true;
      m_pRagdoll->SetRagdollActive(false);
    }
  }
}

void CComponentRagdoll::UpdatePostAnim(float _fDeltaTime)
{
  if(m_bActive)
  {
    m_pRagdoll->Update();
    Mat44f m = m_pRagdoll->GetTransform();
    m_pRAIM->SetMat44(m);



    CPhysxBone *l_pPBone = m_pRagdoll->GetPhysxBoneByName("Bip01 Spine");
    
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
}

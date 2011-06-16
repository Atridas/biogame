#include "ComponentRagdoll.h"
#include "PhysxSkeleton.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"

bool CComponentRagdoll::Init(CGameEntity* _pEntity, const string& _szSkeletonFile)
{
  assert(_pEntity && _pEntity->IsOk());
  SetEntity(_pEntity);

  CComponentRenderableObject* l_pCRO = _pEntity->GetComponent<CComponentRenderableObject>();

  m_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCRO->GetRenderableObject());

  m_pRagdoll = new CPhysxSkeleton(false);
  CalModel* l_pCalModel = m_pRAIM->GetAnimatedInstanceModel()->GetAnimatedCalModel();
  bool l_bOk = m_pRagdoll->Init(_szSkeletonFile,l_pCalModel,m_pRAIM->GetMat44());

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
    m_pRAIM->SetMat44(m_pRagdoll->GetTransform());
  }
}

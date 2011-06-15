#include "ComponentRagdoll.h"
#include "PhysxSkeleton.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"

bool CComponentRagdoll::Init(CGameEntity* _pEntity, const string& _szSkeletonFile)
{
  assert(_pEntity && _pEntity->IsOk());
  SetEntity(_pEntity);

  CComponentRenderableObject* l_pCRO = _pEntity->GetComponent<CComponentRenderableObject>();

  CRenderableAnimatedInstanceModel *l_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCRO->GetRenderableObject());

  m_pRagdoll = new CPhysxSkeleton(false);
  CalModel* l_pCalModel = l_pRAIM->GetAnimatedInstanceModel()->GetAnimatedCalModel();
  bool l_bOk = m_pRagdoll->Init(_szSkeletonFile,l_pCalModel,l_pRAIM->GetMat44());

  SetOk(l_bOk);
  return IsOk();
}

void CComponentRagdoll::Release()
{
  CHECKED_DELETE(m_pRagdoll);
}


void CComponentRagdoll::ActivateRagdoll()
{
  m_pRagdoll->SetRagdollActive(true);
}

void CComponentRagdoll::Update(float _fDeltaTime)
{
  m_pRagdoll->Update();
}
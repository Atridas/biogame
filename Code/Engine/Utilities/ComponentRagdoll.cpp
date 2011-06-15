#include "ComponentRagdoll.h"
#include "PhysxSkeleton.h"
#include "ComponentRenderableObject.h"
#include "RenderableAnimatedInstanceModel.h"

bool CComponentRagdoll::Init(CGameEntity* _pEntity, const string& _szSkeletonFile)
{
  assert(_pEntity && _pEntity->IsOk());

  CComponentRenderableObject* l_pCRO = _pEntity->GetComponent<CComponentRenderableObject>();

  CRenderableAnimatedInstanceModel *l_pRAIM = dynamic_cast<CRenderableAnimatedInstanceModel*>(l_pCRO->GetRenderableObject());

  m_pRagdoll = new CPhysxSkeleton(false);
  CalModel* l_pCalModel = l_pRAIM->GetAnimatedInstanceModel()->GetAnimatedCalModel();
  m_pRagdoll->Init(_szSkeletonFile,l_pCalModel,l_pRAIM->GetMat44());

  SetOk(true);
  return IsOk();
}
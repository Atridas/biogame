#include "ComponentMirilla.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "ComponentRenderableObject.h"
#include "Renderer.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "Component3rdPSCamera.h"
#include "Camera.h"
#include "Core.h"


CComponentMirilla* CComponentMirilla::AddToEntity(CGameEntity *_pEntity)
{
  CComponentMirilla *l_pComp = new CComponentMirilla();
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


bool CComponentMirilla::Init(CGameEntity* _pEntity)
{
  SetOk(true);
  return IsOk();
}

void CComponentMirilla::Release()
{

}

void CComponentMirilla::Enable()
{
   CRenderer *l_pRenderer = CORE->GetRenderer();
   l_pRenderer->ActivateRenderPath("aim_gui");
}

void CComponentMirilla::Disable()
{
  CRenderer *l_pRenderer = CORE->GetRenderer();
   l_pRenderer->DeactivateRenderPath("aim_gui");
}

#include "ComponentMirilla.h"
#include "PhysxSkeleton.h"
#include "PhysxBone.h"
#include "ComponentRenderableObject.h"
#include "Renderer.h"
#include "PostSceneRendererStep.h"
#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "Component3rdPSCamera.h"
#include "Camera.h"
#include "Core.h"


CComponentMirilla* CComponentMirilla::AddToEntity(CGameEntity *_pEntity, const string& _szName)
{
  CComponentMirilla *l_pComp = new CComponentMirilla();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _szName))
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


bool CComponentMirilla::Init(CGameEntity* _pEntity, const string& _szName)
{
  m_pAim = CORE->GetRenderer()->GetPostSceneRendererStep("aim_gui");

  if(m_pAim)
  {
    SetOk(true);
  }else{
    SetOk(false);
  }

  return IsOk();
}

void CComponentMirilla::Release()
{

}

void CComponentMirilla::Enable()
{
  m_pAim->SetActive(true);
}

void CComponentMirilla::Disable()
{
  m_pAim->SetActive(false);
}

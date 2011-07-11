#include "ComponentDoor.h"
#include "ComponentStateMachine.h"
#include "ComponentPhysXBox.h"
#include "ComponentRenderableObject.h"

CComponentDoor* CComponentDoor::AddToEntity(CGameEntity *_pEntity, bool _bOpen)
{
  CComponentDoor *l_pComp = new CComponentDoor();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _bOpen))
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

bool CComponentDoor::Init(CGameEntity* _pEntity, bool _bOpen)
{
  m_bOpen = _bOpen;

  string l_szInitialState = "";
  if(m_bOpen)
    l_szInitialState = "State_Porta_Open";
  else
    l_szInitialState = "State_Porta_Closed";

  //màquina d'estats de la porta:
  if(!CComponentStateMachine::AddToEntity(_pEntity, l_szInitialState))
  {
    LOGGER->AddNewLog(ELL_WARNING, "\tError al carregar la màquina d'estats del component Door.");
    assert(false);
  }

  //correcció d'alçada i posició de l'objecte animat
  CComponentPhysXBox* l_pComponentPhysXBox = dynamic_cast<CComponentPhysXBox*>(_pEntity->GetComponent<CComponentPhysXActor>(ECT_PHYSX_ACTOR));
  CComponentRenderableObject* l_pComponentRenderableObject = _pEntity->GetComponent<CComponentRenderableObject>(ECT_RENDERABLE_OBJECT);

  float l_fTotalHeight = l_pComponentPhysXBox->GetSize().y * 0.5f;

  l_pComponentRenderableObject->m_fHeightAdjustment = -l_fTotalHeight;
  l_pComponentRenderableObject->m_fYawAdjustment = -FLOAT_PI_VALUE / 2;

  SetOk(true);
  return IsOk();
}

bool CComponentDoor::Open()
{
  if(!m_bBlock)
  {
    m_bOpen = true;
    return true;
  }
  return false;
}

bool CComponentDoor::Close()
{
  if(!m_bBlock)
  {
    m_bOpen = false;
    return true;
  }
  return false;
}
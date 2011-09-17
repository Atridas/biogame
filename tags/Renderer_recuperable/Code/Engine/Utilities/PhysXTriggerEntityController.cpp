#define __DONT_INCLUDE_MEM_LEAKS__
#include "PhysicActor.h"
#include "PhysicsManager.h"
#include "EntityDefines.h"
#include "ComponentTrigger.h"
#include "PhysXTriggerEntityController.h"

#include "Utils\MemLeaks.h"

//---CPhysicTrigger Interface---
void CPhysXTriggerEntityController::OnEnter(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape)
{
  CGameEntity* m_pEntityTrigger = entity_trigger1->GetEntity();
  CGameEntity* m_pOtherEntity   = other_shape->GetEntity();

  if(m_pEntityTrigger && m_pOtherEntity)
  {
    CComponentTrigger *l_pCompTrigger = m_pEntityTrigger->GetComponent<CComponentTrigger>(CBaseComponent::ECT_TRIGGER);
    if(l_pCompTrigger)
    {
      l_pCompTrigger->OnEnter(m_pOtherEntity);
    }
  }
}

void CPhysXTriggerEntityController::OnLeave(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape)
{
  CGameEntity* m_pEntityTrigger = entity_trigger1->GetEntity();
  CGameEntity* m_pOtherEntity   = other_shape->GetEntity();

  if(m_pEntityTrigger && m_pOtherEntity)
  {
    CComponentTrigger *l_pCompTrigger = m_pEntityTrigger->GetComponent<CComponentTrigger>(CBaseComponent::ECT_TRIGGER);
    if(l_pCompTrigger)
    {
      l_pCompTrigger->OnExit(m_pOtherEntity);
    }
  }
}
#include "ComponentStateMachine.h"
#include "ScriptedStateMachine.h"
#include "ComponentRenderableObject.h"
#include "ComponentPhysXActor.h"
#include "ComponentPhysXController.h"


CComponentStateMachine* CComponentStateMachine::AddToEntity(CGameEntity* _pEntity, const string& _pEstatInicial)
{
  CComponentStateMachine *l_pComp = new CComponentStateMachine();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _pEstatInicial))
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

bool CComponentStateMachine::Init(CGameEntity* _pEntity, const string& _pEstatInicial)
{
  m_pStateMachine = new CScriptedStateMachine(_pEntity,_pEstatInicial,"");

  m_pStateMachine->SetCurrentState(_pEstatInicial);

  SetOk(true);
  return IsOk();
}


void CComponentStateMachine::Release()
{
  CHECKED_DELETE(m_pStateMachine);
}


void CComponentStateMachine::Update(float _fDeltaTime)
{
  if(m_bActive && m_pStateMachine)
    m_pStateMachine->Update(_fDeltaTime);
}

void CComponentStateMachine::ReceiveEvent(const SEvent& _Event)
{
  if(m_bActive && m_pStateMachine)
    m_pStateMachine->ReceiveEvent(_Event);
}

void CComponentStateMachine::SetActive(bool _bActive)
{
  m_bActive = _bActive;

  CComponentRenderableObject* l_pRO = GetEntity()->GetComponent<CComponentRenderableObject>();

  if(l_pRO)
    l_pRO->SetVisible(m_bActive);

  CComponentPhysXActor* l_pFA = GetEntity()->GetComponent<CComponentPhysXActor>();

  if(l_pFA)
    l_pFA->Activate(m_bActive);

  CComponentPhysXController* l_pFC = GetEntity()->GetComponent<CComponentPhysXController>();

  if(l_pFC)
    l_pFC->Activate(m_bActive);
}

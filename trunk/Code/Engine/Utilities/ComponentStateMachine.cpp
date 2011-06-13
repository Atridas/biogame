#include "ComponentStateMachine.h"
#include "ScriptedStateMachine.h"

bool CComponentStateMachine::Init(CGameEntity* _pEntity, const string& _pEstatInicial)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

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
  m_pStateMachine->Update(_fDeltaTime);
}

void CComponentStateMachine::ReceiveEvent(const SEvent& _Event)
{
  m_pStateMachine->ReceiveEvent(_Event);
}

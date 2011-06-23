#include "ComponentStateMachine.h"
#include "ScriptedStateMachine.h"


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
  m_pStateMachine->Update(_fDeltaTime);
}

void CComponentStateMachine::ReceiveEvent(const SEvent& _Event)
{
  m_pStateMachine->ReceiveEvent(_Event);
}

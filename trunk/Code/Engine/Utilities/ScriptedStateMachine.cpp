#define __DONT_INCLUDE_MEM_LEAKS__

#include "ScriptedStateMachine.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include <luabind/object.hpp>

#include "EntityDefines.h"

#include "Utils/MemLeaks.h"

//use these methods to initialize the FSM
void CScriptedStateMachine::SetCurrentState(luabind::object* s)
{
  m_pCurrentState = s;
}

void CScriptedStateMachine::SetCurrentGlobalState(luabind::object* s)
{
  m_pGlobalState = s;
}

  
//call this to update the FSM
void CScriptedStateMachine::Update(float _fDeltaTime)
{
  //make sure the state is valid before calling its Execute 'method'
  assert (m_pCurrentState && m_pCurrentState->is_valid());  //this could also be written as 'if(m_CurrentState)'
  
  luabind::call_function<void>((*m_pGlobalState)["Update"],m_pOwner,_fDeltaTime);
  
  luabind::call_function<void>((*m_pCurrentState)["Update"],m_pOwner,_fDeltaTime);
}

void CScriptedStateMachine::ReceiveEvent(const SEvent& _Event)
{
  assert (m_pCurrentState && m_pCurrentState->is_valid());

  luabind::call_function<void>((*m_pGlobalState)["Receive"],m_pOwner,_Event);

  luabind::call_function<void>((*m_pCurrentState)["Receive"],m_pOwner,_Event);
}

//change to a new state
void CScriptedStateMachine::ChangeState(luabind::object* _pNewState)
{
  //call the exit method of the existing state
  luabind::call_function<void>((*m_pCurrentState)["Exit"],m_pOwner);

  //change state to the new state
  m_pPreviousState  = m_pCurrentState;
  m_pCurrentState = _pNewState;

  //call the entry method of the new state
  luabind::call_function<void>((*m_pCurrentState)["Enter"],m_pOwner);
}

//change to a new state
void CScriptedStateMachine::ChangeGlobalState(luabind::object* _pNewState)
{
  //call the exit method of the existing state
  luabind::call_function<void>((*m_pGlobalState)["Exit"],m_pOwner);

  //change state to the new state
  m_pPreviousGlobalState  = m_pGlobalState;
  m_pGlobalState = _pNewState;

  //call the entry method of the new state
  luabind::call_function<void>((*m_pGlobalState)["Enter"],m_pOwner);
}

//retrieve the current state
luabind::object* CScriptedStateMachine::CurrentState()const{return m_pCurrentState;}
luabind::object* CScriptedStateMachine::GlobalState()const{return m_pGlobalState;}
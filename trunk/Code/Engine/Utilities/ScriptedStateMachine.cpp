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

  
//call this to update the FSM
void CScriptedStateMachine::Update()
{
  //make sure the state is valid before calling its Execute 'method'
  assert (m_pCurrentState && m_pCurrentState->is_valid());  //this could also be written as 'if(m_CurrentState)'
  
  luabind::call_function<void>((*m_pCurrentState)["Execute"],m_pOwner);
}

void CScriptedStateMachine::ReceiveEvent(const SEvent& _Event)
{
  assert (m_pCurrentState && m_pCurrentState->is_valid());

  luabind::call_function<void>((*m_pCurrentState)["Receive"],m_pOwner,_Event);
}

//change to a new state
void CScriptedStateMachine::ChangeState(luabind::object* _pNewState)
{
  //call the exit method of the existing state
  luabind::call_function<void>((*m_pCurrentState)["Exit"],m_pOwner);

  //change state to the new state
  m_pCurrentState = _pNewState;

  //call the entry method of the new state
  luabind::call_function<void>((*m_pCurrentState)["Enter"],m_pOwner);
}

//retrieve the current state
luabind::object* CScriptedStateMachine::CurrentState()const{return m_pCurrentState;}
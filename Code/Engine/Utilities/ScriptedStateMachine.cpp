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

#include "Core.h"
#include "ScriptManager.h"

#include "Utils/MemLeaks.h"

//use these methods to initialize the FSM
void CScriptedStateMachine::SetCurrentState(const string& s)
{
  m_szCurrentState = s;

  m_bUpdateFailed = m_bReceiveFailed = false;

  
  CScriptManager* l_pSM = CORE->GetScriptManager();
  luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());


  try {
    luabind::call_function<void>(l_GlobalTable[s.c_str()]["Enter"],m_pOwner);
  } catch(const luabind::error& _TheError)
  {
    CScriptManager::PrintError(_TheError);
  }
}

void CScriptedStateMachine::SetCurrentGlobalState(const string& s)
{
  m_szGlobalState = s;

  m_bGlobalUpdateFailed = m_bGlobalReceiveFailed = false;

  CScriptManager* l_pSM = CORE->GetScriptManager();
  luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());

  try {
    luabind::call_function<void>(l_GlobalTable[s.c_str()]["Enter"],m_pOwner);
  } catch(const luabind::error& _TheError)
  {
    CScriptManager::PrintError(_TheError);
  }
}
  
//call this to update the FSM
void CScriptedStateMachine::Update(float _fDeltaTime)
{
  CScriptManager* l_pSM = CORE->GetScriptManager();
  luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());

  if(m_szGlobalState != "")
  {
    try 
    {
      luabind::call_function<void>(l_GlobalTable[m_szGlobalState.c_str()]["Update"],m_pOwner,_fDeltaTime);
    } catch(const luabind::error& _TheError)
    {
      if(!m_bGlobalUpdateFailed)
      {
        CScriptManager::PrintError(_TheError);
        m_bGlobalUpdateFailed = true;
      }
    }
  }
  
  if(m_szCurrentState != "")
  {
    try
    {
      luabind::call_function<void>(l_GlobalTable[m_szCurrentState.c_str()]["Update"],m_pOwner,_fDeltaTime);
    } catch(const luabind::error& _TheError)
    {
      if(!m_bUpdateFailed)
      {
        CScriptManager::PrintError(_TheError);
        m_bUpdateFailed = true;
      }
    }
  }
}

void CScriptedStateMachine::ReceiveEvent(const SEvent& _Event)
{
  CScriptManager* l_pSM = CORE->GetScriptManager();
  luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());

  if(m_szGlobalState != "")
  {
    try
    {
      luabind::call_function<void>(l_GlobalTable[m_szGlobalState.c_str()]["Receive"],m_pOwner,_Event);
    } catch(const luabind::error& _TheError)
    {
      if(!m_bGlobalReceiveFailed)
      {
        CScriptManager::PrintError(_TheError);
        m_bGlobalReceiveFailed = true;
      }
    }
  }

  if(m_szCurrentState != "")
  {
    try
    {
      luabind::call_function<void>(l_GlobalTable[m_szCurrentState.c_str()]["Receive"],m_pOwner,_Event);
    } catch(const luabind::error& _TheError)
    {
      if(!m_bReceiveFailed)
      {
        CScriptManager::PrintError(_TheError);
        m_bReceiveFailed = true;
      }
    }
  }
}

//change to a new state
void CScriptedStateMachine::ChangeState(const string& _szNewState)
{
  CScriptManager* l_pSM = CORE->GetScriptManager();
  luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());

  m_bUpdateFailed = m_bReceiveFailed = false;
  //call the exit method of the existing state
  if(m_szCurrentState != "")
  {
    try
    {
      luabind::call_function<void>(l_GlobalTable[m_szCurrentState.c_str()]["Exit"],m_pOwner);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);
    }
  }

  //change state to the new state
  m_szPreviousState  = m_szCurrentState;
  m_szCurrentState   = _szNewState;

  //call the entry method of the new state
  try
  {
    luabind::call_function<void>(l_GlobalTable[m_szCurrentState.c_str()]["Enter"],m_pOwner);
  } catch(const luabind::error& _TheError)
  {
    CScriptManager::PrintError(_TheError);
  }
}

//change to a new state
void CScriptedStateMachine::ChangeGlobalState(const string& _szNewState)
{
  CScriptManager* l_pSM = CORE->GetScriptManager();
  luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());

  m_bGlobalUpdateFailed = m_bGlobalReceiveFailed = false;
  //call the exit method of the existing state
  if(m_szGlobalState != "")
  {
    try
    {
      luabind::call_function<void>(l_GlobalTable[m_szGlobalState.c_str()]["Exit"],m_pOwner);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);
    }
  }

  //change state to the new state
  m_szPreviousGlobalState  = m_szGlobalState;
  m_szGlobalState = _szNewState;

  //call the entry method of the new state
  try
  {
    luabind::call_function<void>(l_GlobalTable[m_szGlobalState.c_str()]["Enter"],m_pOwner);
  } catch(const luabind::error& _TheError)
  {
    CScriptManager::PrintError(_TheError);
  }
}


void CScriptedStateMachine::RevertState      ()
{
  ChangeState(m_szPreviousState); //TODO testejar això que no m'en fio massa
}

void CScriptedStateMachine::RevertGlobalState()
{
  ChangeState(m_szPreviousGlobalState); //TODO testejar això que no m'en fio massa
}

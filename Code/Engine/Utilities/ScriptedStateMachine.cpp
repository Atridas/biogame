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



static map<int,vector<luabind::object>> g_PreloadedStates;

CScriptedStateMachine::CScriptedStateMachine(CGameEntity* _owner, const string& _szDefaultState, const string& _szDefaultGlobalState):
      m_pOwner(_owner),
      m_bUpdateFailed(false),m_bGlobalUpdateFailed(false),
      m_bReceiveFailed(false),m_bGlobalReceiveFailed(false),
      m_szDefaultState(_szDefaultState),m_szDefaultGlobalState(_szDefaultGlobalState),
      m_pEnter(0),m_pExit(0),m_pUpdate(0),m_pReceive(0),
      m_pGlobalEnter(0),m_pGlobalExit(0),m_pGlobalUpdate(0),m_pGlobalReceive(0)
{
  SetOk(true);
  g_PreloadedStates[m_pOwner->GetGUID()].resize(8);
};

void CScriptedStateMachine::Release()
{
  g_PreloadedStates.erase(m_pOwner->GetGUID());
}


//use these methods to initialize the FSM
void CScriptedStateMachine::SetCurrentState(const string& s)
{
  m_szCurrentState = s;

  m_bUpdateFailed = m_bReceiveFailed = false;

  
  //CScriptManager* l_pSM = CORE->GetScriptManager();
  //luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());
  PreloadState(s, true);
  
  if(m_pEnter)
  {
    try {
      luabind::call_function<void>(*m_pEnter,m_pOwner);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);
    }
  }
}

void CScriptedStateMachine::SetCurrentGlobalState(const string& s)
{
  m_szGlobalState = s;

  m_bGlobalUpdateFailed = m_bGlobalReceiveFailed = false;
  
  PreloadState(s, true);
  
  if(m_pGlobalEnter)
  {
    try {
      luabind::call_function<void>(*m_pGlobalEnter,m_pOwner);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);
    }
  }
}
  
//call this to update the FSM
void CScriptedStateMachine::Update(float _fDeltaTime)
{
  PreloadState(m_szGlobalState, false);
  if(m_pGlobalUpdate)
  {
    try {
      luabind::call_function<void>(*m_pGlobalUpdate,m_pOwner, _fDeltaTime);
    } catch(const luabind::error& _TheError)
    {
      if(!m_bGlobalUpdateFailed)
      {
        CScriptManager::PrintError(_TheError);
        m_bGlobalUpdateFailed = true;
      }
    }
  }

  PreloadState(m_szCurrentState, false);
  if(m_pUpdate)
  {
    try {
      luabind::call_function<void>(*m_pUpdate,m_pOwner, _fDeltaTime);
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
  
  PreloadState(m_szGlobalState, false);
  if(m_pGlobalReceive)
  {
    try {
      luabind::call_function<void>(*m_pGlobalReceive,m_pOwner,_Event);
    } catch(const luabind::error& _TheError)
    {
      if(!m_bGlobalReceiveFailed)
      {
        CScriptManager::PrintError(_TheError);
        m_bGlobalReceiveFailed = true;
      }
    }
  }

  PreloadState(m_szCurrentState, false);
  if(m_pReceive)
  {
    try {
      luabind::call_function<void>(*m_pReceive,m_pOwner,_Event);
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
  m_bUpdateFailed = m_bReceiveFailed = false;
  //call the exit method of the existing state
  if(m_pExit)
  {
    try {
      luabind::call_function<void>(*m_pExit,m_pOwner);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);
    }
  }

  //change state to the new state
  m_szPreviousState  = m_szCurrentState;
  m_szCurrentState   = _szNewState;
  
  PreloadState(m_szCurrentState, false);
  //call the entry method of the new state
  if(m_pEnter)
  {
    try {
      luabind::call_function<void>(*m_pEnter,m_pOwner);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);
    }
  }
}

//change to a new state
void CScriptedStateMachine::ChangeGlobalState(const string& _szNewGlobalState)
{
  m_bUpdateFailed = m_bReceiveFailed = false;
  //call the exit method of the existing state
  if(m_pGlobalExit)
  {
    try {
      luabind::call_function<void>(*m_pGlobalExit,m_pOwner);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);
    }
  }

  //change state to the new state
  m_szPreviousGlobalState  = m_szGlobalState;
  m_szGlobalState          = _szNewGlobalState;
  
  PreloadState(m_szCurrentState, false);
  //call the entry method of the new state
  if(m_pGlobalEnter)
  {
    try {
      luabind::call_function<void>(*m_pGlobalEnter,m_pOwner);
    } catch(const luabind::error& _TheError)
    {
      CScriptManager::PrintError(_TheError);
    }
  }
}


void CScriptedStateMachine::RevertState      ()
{
  string l_szPreviousState = m_szPreviousState;
  ChangeState(l_szPreviousState);
}

void CScriptedStateMachine::RevertGlobalState()
{
  string l_szPreviousGlobalState = m_szPreviousGlobalState;
  ChangeState(l_szPreviousGlobalState);
}


  
void CScriptedStateMachine::PreloadState(const string&    _szNewState, bool _bPrintErrors)
{
  CScriptManager* l_pSM = CORE->GetScriptManager();
  luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());
  luabind::object l_State = l_GlobalTable[_szNewState.c_str()];

  if(l_State)
  {
    PreloadState(l_State, _szNewState, _bPrintErrors);
  }
  else
  {
    if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\"", _szNewState.c_str());
  }
}


void CScriptedStateMachine::PreloadState(luabind::object& _pNewState, const string& _szNewState, bool _bPrintErrors)
{
  vector<luabind::object>& objects = g_PreloadedStates[m_pOwner->GetGUID()];
  
  objects[0] = _pNewState["Enter"];
  objects[1] = _pNewState["Exit"];
  objects[2] = _pNewState["Update"];
  objects[3] = _pNewState["Receive"];

  if(objects[0] && objects[1] && objects[2] && objects[3])
  {
    m_pEnter   = &(objects[0]);
    m_pExit    = &(objects[1]);
    m_pUpdate  = &(objects[2]);
    m_pReceive = &(objects[3]);
  } 
  else 
  {
    m_pEnter   = 0;
    m_pExit    = 0;
    m_pUpdate  = 0;
    m_pReceive = 0;
    if(!objects[0])
    {
      if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\" \"Enter\" function "
        , _szNewState.c_str());
    }
    if(!objects[1])
    {
      if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\" \"Exit\" function "
        , _szNewState.c_str());
    }
    if(!objects[2])
    {
      if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\" \"Update\" function "
        , _szNewState.c_str());
    }
    if(!objects[3])
    {
      if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\" \"Receive\" function "
        , _szNewState.c_str());
    }
  }
}


  
void CScriptedStateMachine::PreloadGlobalState(const string&    _szNewState, bool _bPrintErrors)
{
  CScriptManager* l_pSM = CORE->GetScriptManager();
  luabind::object l_GlobalTable = luabind::globals(l_pSM->GetLuaState());
  luabind::object l_State = l_GlobalTable[_szNewState.c_str()];

  if(l_State)
  {
    PreloadGlobalState(l_State, _szNewState, _bPrintErrors);
  }
  else
  {
    if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\"", _szNewState.c_str());
  }
}


void CScriptedStateMachine::PreloadGlobalState(luabind::object& _pNewState, const string& _szNewState, bool _bPrintErrors)
{
  vector<luabind::object>& objects = g_PreloadedStates[m_pOwner->GetGUID()];
  
  objects[4] = _pNewState["Enter"];
  objects[5] = _pNewState["Exit"];
  objects[6] = _pNewState["Update"];
  objects[7] = _pNewState["Receive"];

  if(objects[4] && objects[5] && objects[6] && objects[7])
  {
    m_pGlobalEnter   = &(objects[4]);
    m_pGlobalExit    = &(objects[5]);
    m_pGlobalUpdate  = &(objects[6]);
    m_pGlobalReceive = &(objects[7]);
  } 
  else 
  {
    m_pGlobalEnter   = 0;
    m_pGlobalExit    = 0;
    m_pGlobalUpdate  = 0;
    m_pGlobalReceive = 0;
    if(!objects[4])
    {
      if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\" \"Enter\" function "
        , _szNewState.c_str());
    }
    if(!objects[5])
    {
      if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\" \"Exit\" function "
        , _szNewState.c_str());
    }
    if(!objects[6])
    {
      if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\" \"Update\" function "
        , _szNewState.c_str());
    }
    if(!objects[7])
    {
      if(_bPrintErrors)
        LOGGER->AddNewLog(ELL_ERROR, "CScriptedStateMachine::PreloadState Can not load state \"%s\" \"Receive\" function "
        , _szNewState.c_str());
    }
  }
}
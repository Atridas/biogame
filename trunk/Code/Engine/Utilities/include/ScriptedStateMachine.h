#pragma once

#ifndef SCRIPTEDScriptedStateMachine_H
#define SCRIPTEDScriptedStateMachine_H
#pragma warning (disable : 4786)

namespace luabind {
  namespace adl {class object;};
  using adl::object;
};
class CGameEntity;
struct SEvent;

#include "base.h"

//template <class entity_type>
class CScriptedStateMachine
{
  
public:

  CScriptedStateMachine(CGameEntity* _owner):m_pOwner(_owner),
    m_bUpdateFailed(false),m_bGlobalUpdateFailed(false),
    m_bReceiveFailed(false),m_bGlobalReceiveFailed(false) {};

  //use these methods to initialize the FSM
  void SetCurrentState      (const string& s);
  void SetCurrentGlobalState(const string& s);

  
  //call this to update the FSM
  void  Update(float _fDeltaTime);


  void ReceiveEvent(const SEvent& _Event);

  //change to a new state
  void  RevertState      ();
  void  RevertGlobalState();

  void  ChangeState      (const string& _pNewState);
  void  ChangeGlobalState(const string& _pNewState);

  //retrieve the current state
  const string&  CurrentState()const {return m_szCurrentState;};
  const string&  GlobalState()const  {return m_szGlobalState;};

private:

  //pointer to the agent that owns this instance
  CGameEntity*      m_pOwner;

  //the current state is a lua table of lua functions. A table may be
  //represented in C++ using a luabind::object
  string  m_szCurrentState;
  string  m_szPreviousState;
  string  m_szGlobalState;
  string  m_szPreviousGlobalState;
  
  bool              m_bUpdateFailed;
  bool              m_bGlobalUpdateFailed;
  bool              m_bReceiveFailed;
  bool              m_bGlobalReceiveFailed;
};




#endif

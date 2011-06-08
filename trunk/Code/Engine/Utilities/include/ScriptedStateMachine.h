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


//template <class entity_type>
class CScriptedStateMachine
{
  
public:

  CScriptedStateMachine(CGameEntity* _owner):m_pOwner(_owner),
  m_pCurrentState(0), m_pPreviousState(0), m_pGlobalState(0), m_pPreviousGlobalState(0) {};

  //use these methods to initialize the FSM
  void SetCurrentState      (luabind::object* s);
  void SetCurrentGlobalState(luabind::object* s);

  
  //call this to update the FSM
  void  Update(float _fDeltaTime);


  void ReceiveEvent(const SEvent& _Event);

  //change to a new state
  void  ChangeState      (luabind::object* _pNewState);
  void  ChangeGlobalState(luabind::object* _pNewState);
  void  RevertState      (luabind::object* _pNewState);
  void  RevertGlobalState(luabind::object* _pNewState);

  //retrieve the current state
  luabind::object*  CurrentState()const;
  luabind::object*  GlobalState()const;

private:

  //pointer to the agent that owns this instance
  CGameEntity*      m_pOwner;

  //the current state is a lua table of lua functions. A table may be
  //represented in C++ using a luabind::object
  luabind::object*   m_pCurrentState;
  luabind::object*   m_pPreviousState;
  luabind::object*   m_pGlobalState;
  luabind::object*   m_pPreviousGlobalState;
};




#endif

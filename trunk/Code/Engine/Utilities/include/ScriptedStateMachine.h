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
#include "Utils/BaseControl.h"
//template <class entity_type>
class CScriptedStateMachine:
  public CBaseControl
{
  
public:

  CScriptedStateMachine(CGameEntity* _owner, const string& _szDefaultState, const string& _szDefaultGlobalState);

  ~CScriptedStateMachine() {Done();}

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
  void  ChangeGlobalState(const string& _pNewGlobalState);

  //retrieve the current state
  const string&  CurrentState()const {return m_szCurrentState;};
  const string&  GlobalState()const  {return m_szGlobalState;};

protected:
  virtual void Release();
private:
  
  void PreloadState(const string&    _szNewState, bool _bPrintErrors);
  void PreloadState(luabind::object& _pNewState, const string& _szNewState, bool _bPrintErrors);
  
  void PreloadGlobalState(const string&    _szNewState, bool _bPrintErrors);
  void PreloadGlobalState(luabind::object& _pNewState, const string& _szNewState, bool _bPrintErrors);

  //pointer to the agent that owns this instance
  CGameEntity*      m_pOwner;

  //the current state is a lua table of lua functions. A table may be
  //represented in C++ using a luabind::object
  string  m_szCurrentState;
  string  m_szPreviousState;
  string  m_szGlobalState;
  string  m_szPreviousGlobalState;

  string  m_szDefaultState, m_szDefaultGlobalState;
  
  bool              m_bUpdateFailed;
  bool              m_bGlobalUpdateFailed;
  bool              m_bReceiveFailed;
  bool              m_bGlobalReceiveFailed;
  
  
  luabind::object*  m_pEnter;
  luabind::object*  m_pExit;
  luabind::object*  m_pUpdate;
  luabind::object*  m_pReceive;
  
  luabind::object*  m_pGlobalEnter;
  luabind::object*  m_pGlobalExit;
  luabind::object*  m_pGlobalUpdate;
  luabind::object*  m_pGlobalReceive;
};




#endif

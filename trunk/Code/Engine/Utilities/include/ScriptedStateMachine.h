#ifndef SCRIPTEDScriptedStateMachine_H
#define SCRIPTEDScriptedStateMachine_H
#pragma warning (disable : 4786)


//Necessita el luabind per a funcionar!
/*
extern "C"
{
  #include <lua.h>
}
*/

class luabind::object;


template <class entity_type, class event_type>
class ScriptedStateMachine
{

private:

  //pointer to the agent that owns this instance
  entity_type*      m_pOwner;

  //the current state is a lua table of lua functions. A table may be
  //represented in C++ using a luabind::object
  luabind::object   m_CurrentState;
  
public:

  ScriptedStateMachine(entity_type* _owner):m_pOwner(_owner){}

  //use these methods to initialize the FSM
  void SetCurrentState(const luabind::object& s){m_CurrentState = s;}

  
  //call this to update the FSM
  void  Update()
  {
    //make sure the state is valid before calling its Execute 'method'
    if (m_CurrentState.is_valid())  //this could also be written as 'if(m_CurrentState)'
    { 
      m_CurrentState.at("Execute")(m_pOwner);
    }
  }

  //change to a new state
  void  ChangeState(const luabind::object& _newState)
  {
    //call the exit method of the existing state
    m_CurrentState.at("Exit")(m_pOwner);

    //change state to the new state
    m_CurrentState = new_state;

    //call the entry method of the new state
    m_CurrentState.at("Enter")(m_pOwner);
  }

  void OnEvent(const event_type& _event)
  {
    m_CurrentState.at("OnEvent")(_event);
  }

  //retrieve the current state
  const luabind::object&  CurrentState()const{return m_CurrentState;}
};




#endif

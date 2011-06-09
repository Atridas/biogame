#pragma once
#ifndef __COMPONENT_STATE_MACHINE__
#define __COMPONENT_STATE_MACHINE__

#include "base.h"
#include "EntityDefines.h"

class CScriptedStateMachine;

class CComponentStateMachine:
  public CBaseComponent
{
public:
  CComponentStateMachine() {};
  ~CComponentStateMachine() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_STATE_MACHINE;};

  bool Init(CGameEntity* _pEntity);
  
  virtual void Update(float _fDeltaTime);
  virtual void ReceiveEvent(const SEvent& _Event);

  CScriptedStateMachine* GetStateMachine() const { return m_pStateMachine; };

protected:
  virtual void Release();
private:
  CScriptedStateMachine* m_pStateMachine;
};

#endif
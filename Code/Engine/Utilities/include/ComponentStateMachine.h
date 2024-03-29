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
  ~CComponentStateMachine() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_STATE_MACHINE;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_STATE_MACHINE;};

  static CComponentStateMachine* AddToEntity(CGameEntity* _pEntity, const string& _pEstatInicial);
  
  virtual void Update(float _fDeltaTime);
  virtual void ReceiveEvent(const SEvent& _Event);

  CScriptedStateMachine* GetStateMachine() const { return m_pStateMachine; };
  
  virtual bool UpdateInPause() const {return m_bUpdatePause;};
  
  bool m_bUpdatePause;
protected:
  CComponentStateMachine(): m_pStateMachine(0), m_bUpdatePause(false) {};
  bool Init(CGameEntity* _pEntity, const string& _pEstatInicial);

  virtual void Release();
private:

  CScriptedStateMachine* m_pStateMachine;
};

#endif
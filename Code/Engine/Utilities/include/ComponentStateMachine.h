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

  void SetActive(bool _bActive);

  CScriptedStateMachine* GetStateMachine() const { return m_pStateMachine; };

protected:
  CComponentStateMachine():m_bActive(true), m_pStateMachine(0) {};
  bool Init(CGameEntity* _pEntity, const string& _pEstatInicial);
  virtual void Release();
private:
  bool m_bActive;
  CScriptedStateMachine* m_pStateMachine;
};

#endif
#pragma once
#ifndef __COMPONENT_INTERACTIVE__
#define __COMPONENT_INTERACTIVE__

#include "base.h"
#include "EntityDefines.h"

class CComponentInteractive:
  public CBaseComponent
{
public:
  ~CComponentInteractive() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_INTERACTIVE;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_INTERACTIVE;};

  static CComponentInteractive* AddToEntity(CGameEntity* _pEntity, const string& _szAction);
  
  virtual void ReceiveEvent(const SEvent& _Event);

protected:
  CComponentInteractive():m_szAction("") {};
  bool Init(CGameEntity* _pEntity, const string& _szAction);
  virtual void Release() {};

private:
  void CallAction(int _iCaller);

  string m_szAction;
};

#endif
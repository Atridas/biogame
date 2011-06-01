#pragma once
#ifndef __COMPONENT_TRIGGER__
#define __COMPONENT_TRIGGER__

#include "base.h"
#include "EntityDefines.h"

class CComponentTrigger:
  public CBaseComponent
{
public:
  CComponentTrigger() {};
  ~CComponentTrigger(){Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_TRIGGER;};

  void Init(CGameEntity* _pEntity, const string& _szOnEnter, const string& _szOnExit);
  
  void OnEnter(CGameEntity* _pOther);
  void OnExit (CGameEntity* _pOther);

protected:
  virtual void Release() {};

private:
  string m_szOnEnter, m_szOnExit;
};

#endif
#pragma once
#ifndef __COMPONENT_DESTROYABLE__
#define __COMPONENT_DESTROYABLE__

#include "base.h"
#include "EntityDefines.h"

class CComponentDestroyable:
  public CBaseComponent
{
public:
  ~CComponentDestroyable() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_DESTROYABLE;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_DESTROYABLE;};

  static CComponentDestroyable* AddToEntity(CGameEntity* _pEntity, const string& _szAction, const string& _szResource);
  
  virtual void ReceiveEvent(const SEvent& _Event);

protected:
  CComponentDestroyable():m_szAction(""), m_szResource(""), m_bDestroyed(false) {};
  bool Init(CGameEntity* _pEntity, const string& _szAction, const string& _szResource);
  virtual void Release() {};

private:
  void CallAction();

  bool m_bDestroyed;

  string m_szAction;
  string m_szResource;
};

#endif
#pragma once
#ifndef __COMPONENT_TRIGGER__
#define __COMPONENT_TRIGGER__

#include "base.h"
#include "ComponentPhysXActor.h"

class CComponentTrigger:
  public CComponentPhysXActor
{
public:
  CComponentTrigger() {};
  ~CComponentTrigger() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_TRIGGER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_TRIGGER;};

  bool Init(CGameEntity* _pEntity, const Vect3f& _vSize, const string& _szOnEnter, const string& _szOnExit, int _iCollisionMask);
  
  void OnEnter(CGameEntity* _pOther);
  void OnExit (CGameEntity* _pOther);


private:
  string m_szOnEnter, m_szOnExit;
};

#endif
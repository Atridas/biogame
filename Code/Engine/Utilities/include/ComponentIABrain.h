#pragma once
#ifndef __COMPONENT_IA_BRAIN__
#define __COMPONENT_IA_BRAIN__

#include "base.h"
#include "EntityDefines.h"

class CComponentIABrain:
  public CBaseComponent
{
public:
  CComponentIABrain():m_pPlayer(0),m_fTime(0) {};
  ~CComponentIABrain() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_IA_BRAIN;};

  bool Init(CGameEntity* _pEntity, const string& _szPlayerEntityName);

  CGameEntity* m_pPlayer;
  float        m_fTime;

protected:
  virtual void Release() {};
};

#endif

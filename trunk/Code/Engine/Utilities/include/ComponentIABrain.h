#pragma once
#ifndef __COMPONENT_IA_BRAIN__
#define __COMPONENT_IA_BRAIN__

#include "base.h"
#include "EntityDefines.h"

class CComponentIABrain:
  public CBaseComponent
{
public:
  ~CComponentIABrain() {Done();}

  static CComponentIABrain* AddToEntity(CGameEntity* _pEntity, const string& _szPlayerEntityName, const string& _szRagdollName);

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_IA_BRAIN;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_IA_BRAIN;};

  void Update(float _fDeltaTime);

  void Shoot();
  void Die();

  CGameEntity* m_pPlayer;
  float        m_fTime;
  bool         m_bShooted;

protected:
  CComponentIABrain():m_pPlayer(0),m_fTime(0),m_bShooted(false),m_iNumUpdates(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szPlayerEntityName, const string& _szRagdollName);

  int m_iNumUpdates;

  virtual void Release() {};

  string m_szRagdollName;
};

#endif

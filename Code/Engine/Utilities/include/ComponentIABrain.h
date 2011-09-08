#pragma once
#ifndef __COMPONENT_IA_BRAIN__
#define __COMPONENT_IA_BRAIN__

#include "base.h"
#include "EntityDefines.h"

class CGraphNode;
class CComponentNavNode;

class CComponentIABrain:
  public CBaseComponent
{
public:
  ~CComponentIABrain() {Done();}

  static CComponentIABrain* AddToEntity(CGameEntity* _pEntity, const string& _szPlayerEntityName, const string& _szRagdollName, const string& _szOnDeathScript = "");

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_IA_BRAIN;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_IA_BRAIN;};

  virtual void ReceiveEvent(const SEvent& _Event);

  void Update(float _fDeltaTime);

  void Shoot();
  void ReceiveShoot(SEvent _sEvent);
  void ReceiveForce(SEvent _sEvent);
  void Die();
  
  //Path planning
  void PlanPathToCobertura();
  Vect3f GetNextNodePosition() const;
  bool ArrivedAtDestination() const;
  bool ArrivedAtNode(float _fDistanceSq) const;
  void SetNextNode();

  CGameEntity* m_pPlayer;
  float        m_fTime;
  bool         m_bShooted;
  vector<CGraphNode*> m_PathToCobertura;

private:
  CComponentIABrain():m_pPlayer(0),m_fTime(0),m_bShooted(false),m_iNumUpdates(0), m_pCover(0), m_szOnDeathScript("") {};
  bool Init(CGameEntity* _pEntity, const string& _szPlayerEntityName, const string& _szRagdollName, const string& _szOnDeathScript = "");

  void RunScript();

  int m_iNumUpdates;

  virtual void Release() {};

  string m_szRagdollName;

  string m_szOnDeathScript;

  CComponentNavNode* m_pCover;
};

#endif

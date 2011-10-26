#pragma once
#ifndef __COMPONENT_IA_BRAIN_VIGIA__
#define __COMPONENT_IA_BRAIN_VIGIA__

#include "base.h"
#include "EntityDefines.h"

#include "Utils\Object3D.h"

class CGraphNode;
class CComponentNavNode;

class CComponentIABrainVigia:
  public CBaseComponent
{
public:
  ~CComponentIABrainVigia() {Done();}

  static CComponentIABrainVigia* AddToEntity(CGameEntity* _pEntity, const string& _szPlayerEntityName, const Vect3f& _vZoneSize, const Mat44f& _mZoneTransform, const string& _szOnDeathScript = "");

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_IA_BRAIN_VIGIA;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_IA_BRAIN_VIGIA;};

  virtual void ReceiveEvent(const SEvent& _Event);

  void SetTargetPosition(const Vect3f& _vPos) { m_vTargetPosition = _vPos;};
  void Fly(bool _bFly);
  void LookAt(const Vect3f& _vPos, float l_fTimeDelta);

  virtual void UpdatePostPhysX(float _fDeltaTime);
  
  virtual void DebugRender(CRenderManager*);

  void Shoot(float _fShootPrecision);
  void ReceiveShoot(SEvent _sEvent);
  void ReceiveForce(SEvent _sEvent);

  void ChooseNewPatrolPosition();

  CGameEntity* m_pPlayer;
  float        m_fTime;
  int          m_iShoots;
  bool         m_bShooted;

  Vect3f       m_vPatrolPosition;
  Vect3f       m_vPatrolDirection;


private:
  CComponentIABrainVigia():m_pPlayer(0),m_fTime(0),m_bShooted(false),m_iShoots(0),m_vTargetPosition(0.0f),m_bFly(false), m_szOnDeathScript(""), m_bDead(false) {};
  bool Init(CGameEntity* _pEntity, const string& _szPlayerEntityName, const Vect3f& _vZoneSize, const Mat44f& _mZoneTransform, const string& _szOnDeathScript = "");

  void Die();
  void RunScript();
  virtual void Enable();

  bool m_bFly;
  bool m_bDead;

  virtual void Release() {};

  string m_szOnDeathScript;

  Vect3f m_vTargetPosition;
  
  CObject3D m_PatrolZone;
};

#endif

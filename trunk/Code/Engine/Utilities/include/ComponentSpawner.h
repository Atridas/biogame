#pragma once
#ifndef __COMPONENT_SPAWNER__
#define __COMPONENT_SPAWNER__

#include "base.h"
#include "EntityDefines.h"

class CComponentObject3D;

class CComponentSpawner:
  public CBaseComponent
{
public:
  enum EEnemyType {
    EE_MILITAR = 0,
    EE_MINER
  };

  ~CComponentSpawner() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_SPAWNER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_SPAWNER;};

  static CComponentSpawner* AddToEntity(CGameEntity* _pEntity, bool _bActive, float _fSpawnTime, int _iMaxEnemy, EEnemyType _EnemyType);
  
  //virtual void ReceiveEvent(const SEvent& _Event);

  void Update(float _fDeltaTime);

  EEnemyType GetEnemyType() { return m_EnemyType; };
  void SetEnemyType(EEnemyType _Type) { m_EnemyType = _Type; };

  bool m_bActive;

protected:
  CComponentSpawner(): m_bActive(false), m_fTime(0.0f), m_fSpawnTime(10.0f), m_iMaxEnemy(10), m_iCurrentEnemy(0), m_EnemyType(EE_MILITAR), m_pCObject3D(0) {};
  bool Init(CGameEntity* _pEntity, bool _bActive, float _fSpawnTime, int _iMaxEnemy, EEnemyType _EnemyType);
  virtual void Release() {};

private:
  float m_fTime;
  float m_fSpawnTime;
  int m_iMaxEnemy;
  int m_iCurrentEnemy;
  EEnemyType m_EnemyType;

  CComponentObject3D* m_pCObject3D;
};

#endif

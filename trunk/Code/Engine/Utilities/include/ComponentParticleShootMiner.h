#pragma once

#ifndef __COMPONENT_PARTICLE_SHOOT_MINER__
#define __COMPONENT_PARTICLE_SHOOT_MINER__

#include "base.h"
#include "BillBoard.h"
#include "EntityDefines.h"

class CComponentParticleShootMiner :
  public CBaseComponent
{
public:
  CComponentParticleShootMiner(){};
  ~CComponentParticleShootMiner(){Done();};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PARTICLE_SHOOT_MINER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_PARTICLE_SHOOT_MINER;};

  static CComponentParticleShootMiner* AddToEntity(CGameEntity* _pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd);
  void Update(float _fDeltaTime);
  bool Init(CGameEntity *_pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd);
  void Release();
  
  
private:
  CBillBoard*                       m_pBillboard;
  Vect3f                            m_vPosInit;
  Vect3f                            m_vPosActual;
  Vect3f                            m_vPosEnd;
  Vect3f                            m_vPosDiferencial1;
  float                             m_fTime;
  bool                              m_bActive;
};
#endif


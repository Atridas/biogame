#pragma once

#ifndef __COMPONENT_PARTICLE_SHOOT_PLAYER__
#define __COMPONENT_PARTICLE_SHOOT_PLAYER__

#include "base.h"
#include "BillBoard.h"
#include "EntityDefines.h"

//------------------------------
/*class CPhysxSkeleton;
class CRenderableAnimatedInstanceModel;
class CRenderableObject;
class CComponentObject3D;
class CComponentMovement;*/
//------------------------------


class CComponentParticleShootPlayer:
  public CBaseComponent
{
public:
  CComponentParticleShootPlayer(){/*m_bBlock=true;*/};
  ~CComponentParticleShootPlayer(){Done();}
  
  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PARTICLE_SHOOT;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_PARTICLE_SHOOT;};

  static CComponentParticleShootPlayer* AddToEntity(CGameEntity* _pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd);
  void Update(float _fDeltaTime);
  bool Init(CGameEntity *_pEntity, const Vect3f& _vPosInit, const Vect3f& _vPosEnd);
  void Release();
  
  
private:
  CBillBoard*                        m_pBillboard;
  Vect3f                            m_vPosInit;
  Vect3f                            m_vPosActual;
  Vect3f                            m_vPosEnd;
  Vect3f                            m_vPosDiferencial1;
  bool                              m_bActive;
  //bool                              m_bBlock;
};
#endif

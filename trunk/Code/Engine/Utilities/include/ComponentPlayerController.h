#pragma once
#ifndef __COMPONENT_PLAYER_CONTROLLER__
#define __COMPONENT_PLAYER_CONTROLLER__

#include "base.h"
#include "EntityDefines.h"
#include "Utils/Object3D.h"

//--- forward declarations
class CComponentObject3D;
class CComponentMovement;
class CRenderableAnimatedInstanceModel;
class CPhysicUserData;
struct SCollisionInfo;
//--------------------

class CComponentPlayerController :
  public CBaseComponent
{
public:



  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PLAYER_CONTROLLER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_PLAYER_CONTROLLER;};

  static CComponentPlayerController* AddToEntity(CGameEntity *_pEntity);

  void Update(float _fDeltaTime);

  virtual ~CComponentPlayerController(void) {Done();};

  //Dades pr�pies dels components
  
  Vect3f m_vCoverNormal;
  Vect3f m_vCoverPosition;
  CGameEntity* m_pCoverEntity;

  float m_fSpeed;
  string m_szCurrentAnimation;
  Vect3f m_vPosInicial;

  float m_fTime;

  void Shoot();
  void Die();
  void Respawn();
  bool Cover();

protected:
  CComponentPlayerController():
      m_pObject3D(0), m_pMovement(0), m_pCoverEntity(0),

      m_pAnimatedModel(0),
      m_szCurrentAnimation(""),
      m_fSpeed(1),

      m_fTime(0),

      m_vCoverNormal(Vect3f(0.0f)),
      m_vCoverPosition(Vect3f(0.0))
     
      {};
  bool Init(CGameEntity *_pEntity);
  virtual void Release() {};

private:
  CPhysicUserData* CheckCover(SCollisionInfo& _sCInfo);
  //Altres components referenciats
  CComponentMovement * m_pMovement;
  CComponentObject3D * m_pObject3D;

  CRenderableAnimatedInstanceModel* m_pAnimatedModel;
};

#endif

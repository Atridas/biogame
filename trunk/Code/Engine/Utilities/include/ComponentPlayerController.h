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

  void UpdatePostPhysX(float _fDeltaTime);

  virtual ~CComponentPlayerController(void) {Done();};

  virtual void ReceiveEvent(const SEvent& _Event);

  //Dades pròpies dels components
  
  Vect3f m_vCoverNormal;
  Vect3f m_vCoverPosition;
  CGameEntity* m_pCoverEntity;

  float m_fSpeed;
  string m_szCurrentAnimation;
  Vect3f m_vPosInicial;

  float m_fTime;

  bool m_bShootActive;
  bool m_bGrenadeActive;
  bool m_bForceActive;

  bool Shoot();
  bool ShootGrenade(float _fTime);
  void Force();
  void Respawn();
  bool Cover();
  void Use();

  float AimDistance();

  bool IsReadyForce() const;

  bool HasPickUp(const string& _szPickUp);
  void AddPickUp(const string& _szPickUp);
  bool RemovePickUp(const string& _szPickUp);

  void SetGodMode(bool _bValue);

private:
  CComponentPlayerController():
      m_bGodMode(false),

      m_pObject3D(0), m_pMovement(0), m_pCoverEntity(0),

      m_pAnimatedModel(0),
      m_szCurrentAnimation(""),
      m_fSpeed(1),

      m_fTime(0),
      m_fBloodFadeOutTime(0.0f),
      m_fBloodTime(0.0f),
      m_fForceTime(0.0f),

      m_vCoverNormal(Vect3f(0.0f)),
      m_vCoverPosition(Vect3f(0.0)),

      m_iNumUpdates(0),

      m_bShootActive(false),
      m_bGrenadeActive(false),
      m_bForceActive(false)
     
      {};
  bool Init(CGameEntity *_pEntity);
  virtual void Release() { m_vPickUps.clear(); };

  void Die();
  void ReceiveForce(SEvent _sEvent);
  void ActivateRagdoll();

  bool m_bGodMode;

  float m_fBloodFadeOutTime;
  float m_fBloodTime;

  float m_fForceTime;

  CPhysicUserData* CheckCover(SCollisionInfo& _sCInfo);
  //Altres components referenciats
  CComponentMovement * m_pMovement;
  CComponentObject3D * m_pObject3D;

  CRenderableAnimatedInstanceModel* m_pAnimatedModel;

  vector<string> m_vPickUps;
  int m_iNumUpdates;

};

#endif

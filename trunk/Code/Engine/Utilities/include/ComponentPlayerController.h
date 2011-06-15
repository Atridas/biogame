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
//--------------------

class CComponentPlayerController :
  public CBaseComponent
{
public:


  CComponentPlayerController():
      m_pObject3D(0), m_pMovement(0),

      m_pAnimatedModel(0),
      m_szCurrentAnimation(""),
      m_fSpeed(1),

      m_fWalkSpeed(1),
      m_fRunSpeed(2),
      m_fYawSpeed(1),
      m_fPitchSpeed(1),

      m_fTime(0),
     
      m_fMaxPitchAngle( FLOAT_PI_VALUE/3),
      m_fMinPitchAngle(-FLOAT_PI_VALUE/3)
     
      {};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PLAYER_CONTROLLER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_PLAYER_CONTROLLER;};

  bool Init(CGameEntity *_pEntity);

  bool Init(CGameEntity *_pEntity,
            const string& _szMoveForward,
            const string& _szMoveBack,
            const string& _szMoveLeft,
            const string& _szMoveRight,

            const string& _szWalk,
            const string& _szRun,

            const string& _szAim,
            const string& _szShoot,

            const string& _szIdleAnimation,
            const string& _szForwardAnimation,
            const string& _szBackAnimation,
            const string& _szLeftAnimation,
            const string& _szRightAnimation,
            const string& _szAimAnimation,
            const string& _szShootAnimation,
  
            float _fWalkSpeed,
            float _fRunSpeed,
            float _fYawSpeed,
            float _fPitchSpeed,
  
            float _fMaxPitchAngle,
            float _fMinPitchAngle);

  void Update(float _fDeltaTime);

  virtual ~CComponentPlayerController(void) {Done();};

  //Dades pròpies dels components
  string m_szMoveForward;
  string m_szMoveBack;
  string m_szMoveLeft;
  string m_szMoveRight;
  
  string m_szRun;
  string m_szWalk;

  string m_szAim;
  string m_szShoot;
  
  string m_szIdleAnimation;
  string m_szForwardAnimation;
  string m_szBackAnimation;
  string m_szLeftAnimation;
  string m_szRightAnimation;
  string m_szAimAnimation;
  string m_szShootAnimation;
  
  float m_fWalkSpeed;
  float m_fRunSpeed;
  float m_fYawSpeed;
  float m_fPitchSpeed;
  
  float m_fMaxPitchAngle;
  float m_fMinPitchAngle;
  

  float m_fSpeed;
  string m_szCurrentAnimation;

  float m_fTime;

  void Shoot();
  void Die();

protected:
  virtual void Release() {};

private:
  //Altres components referenciats
  CComponentMovement * m_pMovement;
  CComponentObject3D * m_pObject3D;

  CRenderableAnimatedInstanceModel* m_pAnimatedModel;
};

#endif

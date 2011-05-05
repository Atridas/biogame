#pragma once
#ifndef __COMPONENT_PLAYER_CONTROLLER__
#define __COMPONENT_PLAYER_CONTROLLER__

#include "base.h"
#include "EntityDefines.h"
#include "Utils/Object3D.h"

//--- forward declarations
class CComponentObject3D;
class CComponentMovement;
//--------------------

class CComponentPlayerController :
  public CBaseComponent
{
public:


  CComponentPlayerController():
      m_pObject3D(0), m_pMovement(0),

      m_fWalkSpeed(1),
      m_fRunSpeed(2),
      m_fYawSpeed(1),
      m_fPitchSpeed(1),
     
      m_fMaxPitchAngle( FLOAT_PI_VALUE/3),
      m_fMinPitchAngle(-FLOAT_PI_VALUE/3),
     
      m_fSpeed(1)
      {};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PLAYER_CONTROLLER;};

  bool Init(CGameEntity *_pEntity);

  bool Init(CGameEntity *_pEntity,
            const string& _szMoveForward,
            const string& _szMoveBack,
            const string& _szMoveLeft,
            const string& _szMoveRight,

            const string& _szWalk,
            const string& _szRun,
  
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
  
  float m_fWalkSpeed;
  float m_fRunSpeed;
  float m_fYawSpeed;
  float m_fPitchSpeed;
  
  float m_fMaxPitchAngle;
  float m_fMinPitchAngle;

protected:
  virtual void Release() {};

private:
  //Altres components referenciats
  CComponentMovement * m_pMovement;
  CComponentObject3D * m_pObject3D;

  float m_fSpeed;
};

#endif

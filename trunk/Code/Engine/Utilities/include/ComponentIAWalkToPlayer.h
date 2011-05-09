#pragma once
#ifndef __COMPONENT_IA_WALK_TO_PLAYER__
#define __COMPONENT_IA_WALK_TO_PLAYER__

#include "base.h"
#include "EntityDefines.h"
#include "Utils/Object3D.h"

//--- forward declarations
class CComponentObject3D;
class CComponentMovement;
//--------------------

class CComponentIAWalkToPlayer :
  public CBaseComponent
{
public:


  CComponentIAWalkToPlayer():
      m_pObject3D(0), m_pMovement(0),

      m_fWalkSpeed(1)
      {};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_IA_WALK_TO_PLAYER;};

  bool Init(CGameEntity *_pEntity,
            const string& _szPlayerEntityName,
            float _fWalkSpeed);

  void Update(float _fDeltaTime);

  virtual ~CComponentIAWalkToPlayer(void) {Done();};

  //Dades pròpies dels components
  float m_fWalkSpeed;

protected:
  virtual void Release() {};

private:
  //Altres components referenciats
  CComponentMovement * m_pMovement;
  CComponentObject3D * m_pObject3D;

  CComponentObject3D* m_pPlayerPosition;
};

#endif

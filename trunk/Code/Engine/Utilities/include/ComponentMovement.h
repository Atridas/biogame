#pragma once
#ifndef __COMPONENT_MOVEMENT__
#define __COMPONENT_MOVEMENT__

#include "base.h"
#include "EntityDefines.h"


class CComponentMovement :
  public CBaseComponent
{
public:



  CBaseComponent::Type GetType() {return CBaseComponent::ECT_MOVEMENT;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_MOVEMENT;};

  static CComponentMovement* AddToEntity(CGameEntity *_pEntity);

  void PreUpdate(float _fDeltaTime);

  virtual ~CComponentMovement(void) {Done();};

  //Dades pròpies dels components
  Vect3f m_vMovement;

protected:
  CComponentMovement() {};
  bool Init(CGameEntity *_pEntity);
  virtual void Release() {};
};

#endif

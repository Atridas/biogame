#pragma once
#ifndef __COMPONENT_ROTATIVE__
#define __COMPONENT_ROTATIVE__

#include "base.h"
#include "EntityDefines.h"

class CComponentPhysXActor;

class CComponentRotative:
  public CBaseComponent
{
public:

  ~CComponentRotative() {Done();}
  
  CBaseComponent::Type GetType() {return CBaseComponent::ECT_ROTATIVE;};
  static CComponentRotative::Type GetStaticType() {return CBaseComponent::ECT_ROTATIVE;};

  static CComponentRotative* AddToEntity(CGameEntity *_pEntity, const Vect3f& _vRotation);

  virtual void UpdatePrePhysX(float _fDeltaTime);

  Vect3f m_vRotation;

protected:
  virtual void Release() {};

private:
  bool Init(CGameEntity* _pEntity, const Vect3f& _vRotation);
  CComponentRotative(): m_vRotation(Vect3f(0.0f)), m_pActor(0) {};

  CComponentPhysXActor* m_pActor;
};

#endif
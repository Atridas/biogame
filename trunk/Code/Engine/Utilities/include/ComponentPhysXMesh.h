#pragma once
#ifndef __COMPONENT_PHYSX_MESH__
#define __COMPONENT_PHYSX_MESH__

#include "base.h"
#include "ComponentPhysXActor.h"

class CComponentPhysXMesh :
  public CComponentPhysXActor
{
public:
  
  static CComponentPhysXMesh* AddToEntity(CGameEntity* _pEntity, float _fDensity, int _iCollisionGroup);
  virtual ~CComponentPhysXMesh(void) {Done();};

protected:
  CComponentPhysXMesh(){};
  bool Init(CGameEntity* _pEntity, float _fDensity, int _iCollisionGroup);
};

#endif

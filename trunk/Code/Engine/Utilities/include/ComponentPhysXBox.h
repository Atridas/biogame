#pragma once
#ifndef __COMPONENT_PHYSX_BOX__
#define __COMPONENT_PHYSX_BOX__

#include "base.h"
#include "ComponentPhysXActor.h"

class CComponentPhysXBox :
  public CComponentPhysXActor
{
public:
  
  bool Init(CGameEntity *_pEntity, float _fDensity, int _iCollisionMask);

  bool Init(CGameEntity *_pEntity,
            float _fSizeX, float _fSizeY, float _fSizeZ,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionMask
            );
};

#endif

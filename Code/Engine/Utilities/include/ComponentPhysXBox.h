#pragma once
#ifndef __COMPONENT_PHYSX_BOX__
#define __COMPONENT_PHYSX_BOX__

#include "base.h"
#include "ComponentPhysXActor.h"

class CComponentPhysXBox :
  public CComponentPhysXActor
{
public:
  
  static CComponentPhysXBox* AddToEntity(CGameEntity *_pEntity, float _fDensity, int _iCollisionGroup);

  static CComponentPhysXBox* AddToEntity(CGameEntity *_pEntity,
            float _fSizeX, float _fSizeY, float _fSizeZ,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionGroup
            );

  const Vect3f& GetSize() { return m_vSize; };
  
protected:
  CComponentPhysXBox(): m_vSize(Vect3f(0.0f)) {};
  bool Init(CGameEntity *_pEntity, float _fDensity, int _iCollisionGroup);

  bool Init(CGameEntity *_pEntity,
            float _fSizeX, float _fSizeY, float _fSizeZ,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionGroup
            );

  Vect3f m_vSize;
};

#endif

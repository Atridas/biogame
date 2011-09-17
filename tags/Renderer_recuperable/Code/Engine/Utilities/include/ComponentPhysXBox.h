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

  virtual void ReceiveEvent(const SEvent& _Event);

  const Vect3f& GetSize() { return m_vSize; };

  virtual void Reload();
  
protected:
  CComponentPhysXBox(): m_vSize(Vect3f(0.0f)), m_iCollisionGroup(0), m_fDensity(.0f) {};
  bool Init(CGameEntity *_pEntity, float _fDensity, int _iCollisionGroup);

  bool Init(CGameEntity *_pEntity,
            float _fSizeX, float _fSizeY, float _fSizeZ,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionGroup
            );

  Vect3f m_vSize;

  int m_iCollisionGroup;
  float m_fDensity;
};

#endif

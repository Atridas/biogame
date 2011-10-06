#pragma once
#ifndef __COMPONENT_PHYSX_SPHERE__
#define __COMPONENT_PHYSX_SPHERE__

#include "base.h"
#include "ComponentPhysXActor.h"

class CComponentPhysXSphere :
  public CComponentPhysXActor
{
public:
  
  static CComponentPhysXSphere* AddToEntity(CGameEntity *_pEntity, float _fDensity, float _fRadius, float _fSkeletonSize, int _iCollisionGroup);

  static CComponentPhysXSphere* AddToEntity(CGameEntity *_pEntity,float _fRadius,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionGroup
            );

  virtual void ReceiveEvent(const SEvent& _Event);

  //const Vect3f& GetSize() { return m_vSize; };

  virtual void Reload();
  
protected:
  CComponentPhysXSphere(): m_iCollisionGroup(0), m_fDensity(.0f),m_fRadius(.0f),m_fSkeletonSize(.0f) {};
  bool Init(CGameEntity *_pEntity, float _fDensity, float _fRadius,float _fSkeletonSize, int _iCollisionGroup);

  bool Init(CGameEntity *_pEntity,float _fRadius,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionGroup
            );

  //Vect3f m_vSize;

  int m_iCollisionGroup;
  float m_fDensity;
  float m_fRadius;
  float m_fSkeletonSize;
};


#endif

#pragma once
#ifndef __COMPONENT_PHYSX_BOX__
#define __COMPONENT_PHYSX_BOX__

#include "base.h"
#include "EntityDefines.h"

//--- forward declarations
class CComponentObject3D;
class CPhysicActor;
class CPhysicUserData;
//--------------------

class CComponentPhysXBox :
  public CBaseComponent
{
public:


  CComponentPhysXBox():
      m_pObject3D(0),
      m_pPhysXBox(0),
      m_pPhysXData(0)
      {};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PHISX_BOX;};
  
  bool Init(CGameEntity *_pEntity, float _fDensity, int _iCollisionMask);

  bool Init(CGameEntity *_pEntity,
            float _fSizeX, float _fSizeY, float _fSizeZ,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionMask
            );

  void UpdatePostPhysX(float _fDeltaTime);

  virtual ~CComponentPhysXBox(void) {Done();};

protected:
  virtual void Release();

private:
  //Altres components referenciats
  CComponentObject3D * m_pObject3D;

  //Dades pròpies dels components
  CPhysicActor       * m_pPhysXBox;
  CPhysicUserData    * m_pPhysXData;
};

#endif

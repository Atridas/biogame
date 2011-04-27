#pragma once
#ifndef __COMPONENT_PHYSX_CONTROLLER__
#define __COMPONENT_PHYSX_CONTROLLER__

#include "base.h"
#include "EntityDefines.h"

//--- forward declarations
class CComponentObject3D;
class CPhysicController;
class CPhysicUserData;
//--------------------

class CComponentPhysXController :
  public CBaseComponent
{
public:


  CComponentPhysXController():
      CBaseComponent(ECT_PHISX_CONTROLLER),
      m_pObject3D(0),
      m_pPhysXController(0),
      m_pPhisXData(0)
      {};

  bool Init(CGameEntity *_Entity,
            float radius, float height, float slope, float skinwidth, 
		        float stepOffset, uint32 collisionGroups 
            );

  void Update(float _fDeltaTime);

  virtual ~CComponentPhysXController(void) {Done();};

protected:
  virtual void Release();

private:
  //Altres components referenciats
  CComponentObject3D * m_pObject3D;

  //Dades pròpies dels components
  CPhysicController  * m_pPhysXController;
  CPhysicUserData    * m_pPhisXData;
};

#endif

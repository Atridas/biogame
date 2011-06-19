#pragma once
#ifndef __COMPONENT_PHYSX_CONTROLLER__
#define __COMPONENT_PHYSX_CONTROLLER__

#include "base.h"
#include "EntityDefines.h"

//--- forward declarations
class CComponentObject3D;
class CComponentMovement;
class CPhysicController;
class CPhysicUserData;
//--------------------

class CComponentPhysXController :
  public CBaseComponent
{
public:


  CComponentPhysXController():
      m_pObject3D(0), m_pMovement(0),
      m_pPhysXController(0),
      m_pPhysXData(0)
      {};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PHYSX_CONTROLLER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_PHYSX_CONTROLLER;};

  bool Init(CGameEntity *_Entity,
            float radius, float height, float slope, float skinwidth, 
		        float stepOffset, uint32 _iCollisionGroup 
            );
  
  void UpdatePrePhysX(float _fDeltaTime);
  void UpdatePostPhysX(float _fDeltaTime);

  void SetHeight(float _fHeight);

  virtual ~CComponentPhysXController(void) {Done();};

protected:
  virtual void Release();

private:
  //Altres components referenciats
  CComponentObject3D * m_pObject3D;
  CComponentMovement * m_pMovement;

  //Dades pròpies dels components
  CPhysicController  * m_pPhysXController;
  CPhysicUserData    * m_pPhysXData;
};

#endif

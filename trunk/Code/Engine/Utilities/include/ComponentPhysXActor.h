#pragma once
#ifndef __COMPONENT_PHYSX_ACTOR__
#define __COMPONENT_PHYSX_ACTOR__

#include "base.h"
#include "EntityDefines.h"

//--- forward declarations
class CComponentObject3D;
class CPhysicActor;
class CPhysicUserData;
//--------------------

class CComponentPhysXActor :
  public CBaseComponent
{
public:

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PHYSX_ACTOR;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_PHYSX_ACTOR;};
  
  CPhysicActor*    GetActor()     {return m_pPhysXActor;}
  CPhysicUserData* GetUserData()  {return m_pPhysXData;}

  virtual void UpdatePostPhysX(float _fDeltaTime);

  virtual ~CComponentPhysXActor(void) {Done();};

  void Activate(bool _bActivate);

protected:

  CComponentPhysXActor():
      m_pObject3D(0),
      m_pPhysXActor(0),
      m_pPhysXData(0)
      {};

  virtual void Release();

  //Altres components referenciats
  CComponentObject3D * m_pObject3D;

  //Dades pròpies dels components
  CPhysicActor       * m_pPhysXActor;
  CPhysicUserData    * m_pPhysXData;
};

#endif
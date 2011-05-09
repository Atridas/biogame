#pragma once
#ifndef __COMPONENT_PHYSX_MESH__
#define __COMPONENT_PHYSX_MESH__

#include "base.h"
#include "EntityDefines.h"

//--- forward declarations
class CComponentObject3D;
class CPhysicActor;
class CPhysicUserData;
//--------------------

class CComponentPhysXMesh :
  public CBaseComponent
{
public:


  CComponentPhysXMesh():
      m_pObject3D(0),
      m_pPhysXMesh(0),
      m_pPhysXData(0)
      {};

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PHISX_MESH;};

  bool Init(CGameEntity* _pEntity, float _fDensity, int _iCollisionMask);

  void UpdatePostPhysX(float _fDeltaTime);

  virtual ~CComponentPhysXMesh(void) {Done();};

protected:
  virtual void Release();

private:
  //Altres components referenciats
  CComponentObject3D * m_pObject3D;

  //Dades pròpies dels components
  CPhysicActor       * m_pPhysXMesh;
  CPhysicUserData    * m_pPhysXData;
};

#endif

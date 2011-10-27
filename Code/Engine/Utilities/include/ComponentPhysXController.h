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

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_PHYSX_CONTROLLER;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_PHYSX_CONTROLLER;};

  static CComponentPhysXController* AddToEntity(CGameEntity *_Entity,
            float radius, float height, float slope, float skinwidth, 
		        float stepOffset, uint32 _iCollisionGroup 
            );
  
  void UpdatePrePhysX(float _fDeltaTime);
  void UpdatePostPhysX(float _fDeltaTime);

  void SetPositionFromRagdoll();
  void SetHeight(float _fHeight);

  void UseGravity(bool _bValue);

  //No utilitzar a excepció de casos extranys, moure amb el component movement
  void   SetPosition(const Vect3f& _vPosition);
  Vect3f GetPosition() const;

  virtual ~CComponentPhysXController(void) {Done();};

protected:
  CComponentPhysXController():
      m_pObject3D(0), m_pMovement(0),
      m_pPhysXController(0),
      m_pPhysXData(0)
      {};
  bool Init(CGameEntity *_Entity,
            float radius, float height, float slope, float skinwidth, 
		        float stepOffset, uint32 _iCollisionGroup 
            );

  virtual void Enable();
  virtual void Disable();

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

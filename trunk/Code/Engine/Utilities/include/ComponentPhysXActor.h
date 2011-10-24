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

  Vect3f GetPosition();
  //Vect3f GetRotation();
  void SetPosition(Vect3f _vPos);
  void MovePosition(Vect3f _vPos);
  //void SetRotation(Vect3f _vRot);

  virtual void UpdatePostPhysX(float _fDeltaTime);
  virtual void ReceiveEvent(const SEvent& _Event);

  virtual ~CComponentPhysXActor(void) {Done();};

  virtual void Reload() {};


  string m_szParticulaImpacte;
protected:

  CComponentPhysXActor():
      m_pObject3D(0),
      m_pPhysXActor(0),
      m_pPhysXData(0),
      m_szParticulaImpacte("impacte")
      {};

  virtual void Enable();
  virtual void Disable();

  virtual void Release();

  //Altres components referenciats
  CComponentObject3D * m_pObject3D;

  //Dades pròpies dels components
  CPhysicActor       * m_pPhysXActor;
  CPhysicUserData    * m_pPhysXData;
};

#endif
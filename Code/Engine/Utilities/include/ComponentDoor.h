#pragma once
#ifndef __COMPONENT_Door__
#define __COMPONENT_Door__

#include "base.h"
#include "EntityDefines.h"

class CPhysicActor;
class CPhysicUserData;
class CComponentObject3D;

class CComponentDoor:
  public CBaseComponent
{
public:

  ~CComponentDoor() {Done();}
  
  CBaseComponent::Type GetType() {return CBaseComponent::ECT_DOOR;};
  static CComponentDoor::Type GetStaticType() {return CBaseComponent::ECT_DOOR;};

  static CComponentDoor* AddToEntity(CGameEntity *_pEntity, bool _bOpen, Vect3f _vSize);

  void Block(bool _bBlock) { m_bBlock = _bBlock; };
  bool IsBlocked() {return m_bBlock;};

  float m_fTime;

protected:

  virtual void Enable(void);
  virtual void Disable(void);

  virtual void Release();

private:
  bool Init(CGameEntity* _pEntity, bool _bOpen, Vect3f _vSize);
  CComponentDoor(): m_bBlock(false), m_fTime(0.0f),m_pPhysXActor(0),m_pPhysXData(0) {};

  bool m_bBlock;

  Vect3f m_vSizeBox;

  Vect3f m_vPosBox;

  CPhysicActor* m_pPhysXActor;
  CPhysicUserData* m_pPhysXData;

  CComponentObject3D* m_pObject3D;

};

#endif
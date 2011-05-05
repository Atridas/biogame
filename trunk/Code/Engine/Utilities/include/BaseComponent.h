#pragma once

#ifndef __BASE_COMPONENT_H__
#define __BASE_COMPONENT_H__

// NO INCLOURE AQUEST ARXIU !!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!

// incloure "EntityDefines.h"

#include "base.h"

#ifndef __ENTITY_DEFINES_H__
#error S'ha d'incloure "EntityDefines.h" i no "BaseComponent.h"
#endif

#include "EntityDefines.h"

//---- forward declarations --
class CGameEntity;
// ---------------------------

class CBaseComponent:
  public CBaseControl
{
public:
  enum Type {
    ECT_OBJECT_3D,
    ECT_MOVEMENT,
    ECT_PLAYER_CONTROLLER,
    ECT_PHISX_CONTROLLER,
    ECT_PHISX_BOX,
    ECT_3RD_PERSON_SHOOTER_CAMERA
  };
  
  virtual Type GetType() = 0; //{return m_Type;};
  bool IsType(Type _Type) {return GetType() == _Type;};
  CGameEntity* GetEntity() const {return m_pEntity;};
  
  void ReceiveEvent(const SEvent& _Event) {};

  //updates
  
  virtual void PreUpdate(float _fDeltaTime)        {};
  virtual void Update(float _fDeltaTime)           {};
  virtual void UpdatePrePhysX(float _fDeltaTime)   {};
  virtual void UpdatePostPhysX(float _fDeltaTime)  {};
  virtual void UpdatePostAnim(float _fDeltaTime)   {};
  virtual void PostUpdate(float _fDeltaTime)       {};

protected:
  CBaseComponent():m_pEntity(0) {};
  void SetEntity(CGameEntity* _pEntity);

private:
  //EComponentType m_Type;
  CGameEntity*   m_pEntity;
};




#endif
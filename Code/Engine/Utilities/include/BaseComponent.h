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
class CRenderManager;
// ---------------------------

class CBaseComponent:
  public CBaseControl
{
public:
  enum Type {
    ECT_OBJECT_3D,
    ECT_MOVEMENT,
    ECT_PLAYER_CONTROLLER,
    ECT_IA_WALK_TO_PLAYER,
    ECT_PHYSX_CONTROLLER,
    ECT_PHYSX_ACTOR,
    ECT_RENDERABLE_OBJECT,
    ECT_3RD_PERSON_SHOOTER_CAMERA,
    ECT_TRIGGER,
    ECT_VIDA,
    ECT_LASER,
    ECT_STATE_MACHINE,
    ECT_ANIMATION,
    ECT_IA_BRAIN,
    ECT_RAGDOLL,
    ECT_COVER,
    ECT_MIRILLA
  };
  
  virtual Type GetType() = 0; //{return m_Type;};
  bool IsType(Type _Type) {return GetType() == _Type;};
  CGameEntity* GetEntity() const {return m_pEntity;};
  
  virtual void ReceiveEvent(const SEvent& _Event) {};

  //updates
  
  virtual void PreUpdate(float _fDeltaTime)        {};
  virtual void Update(float _fDeltaTime)           {};
  virtual void UpdatePrePhysX(float _fDeltaTime)   {};
  virtual void UpdatePostPhysX(float _fDeltaTime)  {};
  virtual void UpdatePostAnim(float _fDeltaTime)   {};
  virtual void PostUpdate(float _fDeltaTime)       {};


  virtual void DebugRender(CRenderManager*)        {};

protected:
  CBaseComponent():m_pEntity(0) {};
  void SetEntity(CGameEntity* _pEntity);

private:
  //EComponentType m_Type;
  CGameEntity*   m_pEntity;
};




#endif
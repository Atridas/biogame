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
    ECT_ROTATIVE,
    ECT_PLAYER_CONTROLLER,
    ECT_IA_WALK_TO_PLAYER,
    ECT_PHYSX_CONTROLLER,
    ECT_PHYSX_ACTOR,
    ECT_ANIMATION,
    ECT_RENDERABLE_OBJECT,
    ECT_3RD_PERSON_SHOOTER_CAMERA,
    ECT_TRIGGER,
    ECT_ENERGY,
    ECT_LASER,
    ECT_IA_BRAIN,
    ECT_IA_BRAIN_VIGIA,
    ECT_VIDA,
    ECT_DOOR,
    ECT_INTERACTIVE,
    ECT_DESTROYABLE,
    ECT_SPAWNER,
    ECT_STATE_MACHINE,
    ECT_BGM,
    ECT_SOUND,
    ECT_RAGDOLL,
    ECT_COVER,
    ECT_MIRILLA,
    ECT_SHIELD,
    ECT_ARMA,
    ECT_NAV_NODE,
    ECT_EMITER,
    ECT_BILLBOARD,
    ECT_OMNI,
    ECT_PARTICLE_SHOOT_PLAYER,
    ECT_PARTICLE_SHOOT_MINER,
    ECT_PARTICLE_SHOOT_MILITAR,
    ECT_COLLISION_REPORT,
    ECT_EXPLOSIVE,
    ECT_LIFETIME,
    ECT_DELAYED_SCRIPT,
    ECT_SOUND_LISTENER,
    ECT_PHYSXSPHERE,
    ECT_CYNEMATIC_CAMERA
  };
  
  virtual Type GetType() = 0; //{return m_Type;};
  bool IsType(Type _Type) {return GetType() == _Type;};
  CGameEntity* GetEntity() const {return m_pEntity;};
  
  virtual void ReceiveEvent(const SEvent& _Event) {};

  void SetActive(bool _bActive);
  bool IsActive() {return m_bActive;};

  //updates
  
  virtual void PreUpdate(float _fDeltaTime)        {};
  virtual void Update(float _fDeltaTime)           {};
  virtual void UpdatePrePhysX(float _fDeltaTime)   {};
  virtual void UpdatePostPhysX(float _fDeltaTime)  {};
  virtual void UpdatePostAnim(float _fDeltaTime)   {};
  virtual void PostUpdate(float _fDeltaTime)       {};


  virtual void DebugRender(CRenderManager*)        {};

  virtual bool UpdateInPause() const {return false;};

protected:
  CBaseComponent():m_pEntity(0),m_bActive(true) {};
  void SetEntity(CGameEntity* _pEntity);

  virtual void Enable(void) {};
  virtual void Disable(void) {};

private:

  bool m_bActive;

  //EComponentType m_Type;
  CGameEntity*   m_pEntity;
};

#endif
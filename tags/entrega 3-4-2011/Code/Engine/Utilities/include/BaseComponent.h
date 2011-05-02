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

  enum EComponentType {
    ECT_PHISX_CONTROLLER,
    ECT_OBJECT_3D
  };
  
  EComponentType GetType() {return m_Type;};
  bool IsType(EComponentType _Type) {return m_Type == _Type;};
  CGameEntity* GetEntity() const {return m_pEntity;};
  
  void ReceiveEvent(const SEvent& _Event) {};

  //updates
  virtual void Update(float deltaTime) {};

protected:
  CBaseComponent(EComponentType _Type):m_Type(_Type), m_pEntity(0){};
  void SetEntity(CGameEntity* _pEntity) {m_pEntity = _pEntity;};

private:
  EComponentType m_Type;
  CGameEntity*   m_pEntity;
};




#endif
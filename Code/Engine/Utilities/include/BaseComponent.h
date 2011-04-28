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
  
  virtual EComponentType GetType() = 0; //{return m_Type;};
  bool IsType(EComponentType _Type) {return GetType() == _Type;};
  CGameEntity* GetEntity() const {return m_pEntity;};
  
  void ReceiveEvent(const SEvent& _Event) {};

  //updates
  virtual void Update(float _fDeltaTime) {};

protected:
  CBaseComponent():m_pEntity(0) {};
  void SetEntity(CGameEntity* _pEntity);

private:
  //EComponentType m_Type;
  CGameEntity*   m_pEntity;
};




#endif
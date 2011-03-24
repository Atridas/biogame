#pragma once

#ifndef __BASE_COMPONENT_H__
#define __BASE_COMPONENT_H__

#include "base.h"
#include "EntityDefines.h"
#include "GameEntity.h"

//---- forward declarations --
class CGameEntity;
// ---------------------------

class CBaseComponent:
  public CBaseControl
{
public:
  
  EComponentType GetType() {return m_Type;};
  bool IsType(EComponentType _Type) {return m_Type == _Type;};
  CGameEntity* GetEntity() const {return m_pEntity;};
  //TODO events

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
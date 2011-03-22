#pragma once

#ifndef __BASE_COMPONENT_H__
#define __BASE_COMPONENT_H__

#include "base.h"
#include "EntityDefines.h"
#include "GameEntity.h"

class CBaseComponent:
  public CBaseControl
{
public:
  
  EComponentType GetType() {return m_Type;};
  bool IsType(EComponentType _Type) {return m_Type == _Type;};

  //TODO events

  //updates
  virtual void Update(float deltaTime) {};

protected:
  CBaseComponent(EComponentType _Type):m_Type(_Type){};


private:
  EComponentType m_Type;
};




#endif
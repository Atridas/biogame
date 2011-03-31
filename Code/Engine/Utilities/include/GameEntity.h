#pragma once

#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

// NO INCLOURE AQUEST ARXIU !!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!

// incloure "EntityDefines.h"

#include "base.h"

#ifndef __ENTITY_DEFINES_H__
#error S'ha d'incloure "EntityDefines.h" i no "GameEntity.h"
#endif

class CBaseComponent;

class CGameEntity:
  public CBaseControl
{
public:
  CGameEntity():m_iGUID(-1) {};
  virtual ~CGameEntity() {Done();};

  void Init();

  int GetGUID() const {return m_iGUID;};

  CBaseComponent* GetComponent(CBaseComponent::EComponentType _type) const;

  void Update(float deltaTime);

  void ReceiveEvent(const SEvent& _Event);

private:
  map<CBaseComponent::EComponentType, CBaseComponent*> m_vComponents;
  int                     m_iGUID;
};




#endif

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
class CEntityManager;

class CGameEntity:
  public CBaseControl
{
public:

  int GetGUID() const {return m_iGUID;};

  CBaseComponent* GetComponent(CBaseComponent::Type _type) const;

  void Update(float deltaTime);

  void ReceiveEvent(const SEvent& _Event);

protected:

  virtual void Release();

private:
  CGameEntity(int _iId):m_iGUID(_iId) {SetOk(true);};
  ~CGameEntity() {Done();};
  void AddComponent(CBaseComponent* _pComponent);

  map<CBaseComponent::Type, CBaseComponent*> m_vComponents;
  int                     m_iGUID;
  
  friend class CBaseComponent;
  friend class CEntityManager;
};




#endif

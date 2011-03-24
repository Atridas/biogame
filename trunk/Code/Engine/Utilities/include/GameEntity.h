#pragma once

#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "base.h"
#include "BaseComponent.h"
#include "EntityDefines.h"

//---- forward declarations --
class CBaseComponent;
enum EComponentType;
// ---------------------------

class CGameEntity:
  public CBaseControl
{
public:
  CGameEntity():m_iGUID(-1) {};
  virtual ~CGameEntity() {Done();};

  void Init();

  int GetGUID() const {return m_iGUID;};

  CBaseComponent* GetComponent(EComponentType _type) const;

  void Update(float deltaTime);

private:
  map<EComponentType, CBaseComponent*> m_vComponents;
  int                     m_iGUID;
};




#endif

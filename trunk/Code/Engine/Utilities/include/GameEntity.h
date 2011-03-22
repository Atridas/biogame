#pragma once

#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "base.h"
#include "BaseComponent.h"

class CGameEntity:
  public CBaseControl
{
public:
  CGameEntity() {};
  virtual ~CGameEntity() {Done();};

  void Init() {SetOk(true);};

  void Update(float deltaTime);

private:
  vector<CBaseComponent*> m_vComponents;
};




#endif

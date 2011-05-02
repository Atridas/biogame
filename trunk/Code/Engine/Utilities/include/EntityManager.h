#pragma once

#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "base.h"
#include "EntityDefines.h"

class CEntityManager:
  public CBaseControl
{
public:
  CEntityManager() {SetOk(true);};
  ~CEntityManager() {Done();};
  
  CGameEntity* CreateEntity();
  void         SetName(const string&,int);
  CGameEntity* GetEntity(int) const;
  CGameEntity* GetEntity(const string&) const;
  
  void RemoveEntity(int);

protected:
  virtual void Release();

private:
  vector<CGameEntity*> m_vEntities;
  vector<int>          m_vFreeIDs;
  map<string,int>      m_vNames;
};

#endif
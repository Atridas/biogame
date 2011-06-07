#pragma once

#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "base.h"
#include "EntityDefines.h"
#include <stack>

class CEntityManager:
  public CBaseControl
{
public:
  CEntityManager() {SetOk(true);};
  ~CEntityManager() {Done();};
  
  void LoadEntitiesFromXML(const string& _szFile);

  CGameEntity* CreateEntity();
  void         SetName(const string&,int);
  void         SetName(const string&,const CGameEntity*);
  CGameEntity* GetEntity(int) const;
  CGameEntity* GetEntity(const string&) const;
  
  void         PreUpdate(float _fDeltaTime)      ;
  void         Update(float _fDeltaTime)         ;
  void         UpdatePrePhysX(float _fDeltaTime) ;
  void         UpdatePostPhysX(float _fDeltaTime);
  void         UpdatePostAnim(float _fDeltaTime) ;
  void         PostUpdate(float _fDeltaTime)     ;

  void         DebugRender(CRenderManager*)      ;
  
  void RemoveEntity(const CGameEntity*);
  void RemoveEntity(int);
  
  void SendEvent(const SEvent& _Event);

protected:
  virtual void Release();

private:
  
  void RemoveEntities();
  void CreateEntities();
  void SendEvents(float _fDeltaTime);

  vector<CGameEntity*> m_vEntities;
  vector<int>          m_vFreeIDs;
  map<string,int>      m_vNames;
  
  vector<CGameEntity*> m_vNewEntities;
  stack<int>           m_vDeleteEntities;

  vector<SEvent>       m_vEvents;
};

#endif
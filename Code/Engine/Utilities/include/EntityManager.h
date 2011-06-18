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

  int          GetMaxHistoricEntities() {return m_vEntities.size() + m_vNewEntities.size();};

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

  
  // Loading functions
  void LoadEntitiesFromXML(const string& _szFile);

  CGameEntity* InitPlayer(const string& _szEntityName, const Vect3f& _vPosition, float _fYaw);

  CGameEntity* InitEnemy(const string& _szPlayerName, const Vect3f& _vPosition,
                         const string& _szInitialState, const string& _szRenderableModel, const string& _szRagdollModell,
                         const string& _szEntityName = "");

  CGameEntity* InitMiner(const string& _szPlayerName, const Vect3f& _vPosition, const string& _szEntityName = "");

  CGameEntity* InitMilitar(const string& _szPlayerName, const Vect3f& _vPosition, const string& _szEntityName = "");

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
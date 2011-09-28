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
  bool         SetName(const string&,int);
  bool         SetName(const string&,const CGameEntity*);
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
  
  SEvent GetEvent() { return m_EmptyEvent; };
  void SendEvent(const SEvent& _Event);

  
  // Loading functions
  void LoadEntitiesFromXML(const string& _szFile);

  CGameEntity* InitPlayer(const string& _szEntityName, const Vect3f& _vPosition, float _fYaw, bool _bForceActive = true, bool _bShootActive = true);

  CGameEntity* InitEnemy(const string& _szPlayerName, const Vect3f& _vPosition, float _fRadius,
                         const string& _szInitialState, const string& _szRenderableModel, const string& _szRagdollModell,
                         const string& _szEntityName = "", const string& _szOnDeathScript = "");

  CGameEntity* InitMiner(const string& _szPlayerName, const Vect3f& _vPosition, const string& _szEntityName = "", const bool _bActive = true, const string& _szOnDeathScript = "");

  CGameEntity* InitMilitar(const string& _szPlayerName, const Vect3f& _vPosition, const string& _szEntityName = "", const bool _bActive = true, const string& _szOnDeathScript = "");

  CGameEntity* InitLaser(const Vect3f& _vPosInit, const Vect3f& _vDir, float _fDany, uint32 _uiCollisionMask);

  CGameEntity* InitParticles(const string& _szCore, const Vect3f& _vPos, const Vect3f& _vSize, float _fTime = 0, const Vect3f& _vYdir = Vect3f(0,1,0));

  CGameEntity* InitTriggerWithParticles(const string& _szCore, const Vect3f& _vPos, const Vect3f& _vSize, uint32 _uiCollisionMask, const string& _szOnEnter = "", const string& _szOnExit = "", float _fTime = 0, const Vect3f& _vYdir = Vect3f(0,1,0));

  CGameEntity* InitPickUp(const string& _szName, const string& _szCore, const Vect3f& _vPos, const string& _szOnPickUp);

protected:
  virtual void Release();

private:
  
  void RemoveEntities();
  void CreateEntities();
  void SendEvents(float _fDeltaTime);
  void DeliverEvent(const SEvent& _Event);

  SEvent m_EmptyEvent;

  vector<CGameEntity*> m_vEntities;
  vector<int>          m_vFreeIDs;
  map<string,int>      m_vNames;
  
  vector<CGameEntity*> m_vNewEntities;
  stack<int>           m_vDeleteEntities;

  vector<SEvent>       m_vEvents;
};

#endif
#pragma once
#ifndef __PORTAL_MANAGER_H__
#define __PORTAL_MANAGER_H__

#include "base.h"
#include "Room.h"
#include "Portal.h"

#define PORTAL_MANAGER_UPDATE_PERIOD 1.f / 5.f

class CRenderManager;
class CRenderableObject;

class CPortalManager:
  public CBaseControl
{
public:
  CPortalManager():m_fLastUpdate(0) {};
  ~CPortalManager() {Done();};
  
  bool Init(const string& _szFileName);
  bool Init(CXMLTreeNode& _xmlLevel);
  
  CRoom*             GetRoom  (const string& _szName);
  CPortal*           GetPortal(const string& _szName);
  const set<string>& GetRoomNames() const {return m_RoomNames;};
  
  void InsertRenderableObject(CRenderableObject* _pRO);
  void RemoveRenderableObject(CRenderableObject* _pRO);

  void Update(float _fDT);
  void Render(CRenderManager* _pRM) {Render(_pRM,false);};
  void DebugRender(CRenderManager* _pRM);

protected:
  virtual void Release() {m_UnlocatedROs.Done();m_Rooms.clear();m_Portals.clear();};
private:
  void Render(CRenderManager* _pRM, bool _bDebug);

  CRoom               m_UnlocatedROs;

  set<string>         m_RoomNames;
  map<string,CRoom>   m_Rooms;
  map<string,CPortal> m_Portals;

  string              m_szCameraLastRoom;

  float               m_fLastUpdate;
};

#endif

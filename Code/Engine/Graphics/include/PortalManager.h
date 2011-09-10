#pragma once
#ifndef __PORTAL_MANAGER_H__
#define __PORTAL_MANAGER_H__

#include "base.h"
#include "Room.h"
#include "Portal.h"

#define PORTAL_MANAGER_UPDATE_PERIOD 1.f / 5.f

class CRenderManager;
class CRenderableObject;
class CEmiterInstance;
class CCamera;

class CPortalManager:
  public CBaseControl
{
public:
  CPortalManager():m_fLastUpdate(0) {};
  ~CPortalManager() {Done();};
  
  bool Init(const vector<string>& _szFileNames);
  bool Init(const string& _szFileName);
  bool Init(CXMLTreeNode& _xmlLevel);
  
  CRoom*             GetRoom  (const string& _szName);
  CPortal*           GetPortal(const string& _szName);
  const set<string>& GetRoomNames() const {return m_RoomNames;};
  
  void InsertRenderableObject(CRenderableObject* _pRO);
  void RemoveRenderableObject(CRenderableObject* _pRO);
  
  void InsertEmiter(CEmiterInstance* _pEmiter);
  void RemoveEmiter(CEmiterInstance* _pEmiter);

  void Update(float _fDT);
  void Render(CRenderManager* _pRM) {Render(_pRM,false);};
  void GetRenderedObjects(CCamera *_pCamera, vector<CRenderableObject*>& OpaqueObjects_, CRoom::TBlendQueue& BlendQueue_, CRoom::TBlendQueue& EmiterQueue_);
  void DebugRender(CRenderManager* _pRM);

protected:
  virtual void Release();
private:
  void Render(CRenderManager* _pRM, bool _bDebug);
  void ReadRooms(CXMLTreeNode& _xmlRooms, set<string>& _UsedGameObjects);
  void ReadPortals(CXMLTreeNode& _xmlPortals);

  CRoom               m_UnlocatedROs;

  set<string>         m_RoomNames;
  map<string,CRoom>   m_Rooms;
  map<string,CPortal> m_Portals;

  string              m_szCameraLastRoom;

  float               m_fLastUpdate;
};

#endif

#pragma once
#ifndef __PORTAL_MANAGER_H__
#define __PORTAL_MANAGER_H__

#include "base.h"
#include "Room.h"
#include "Portal.h"

class CRenderManager;

class CPortalManager:
  public CBaseControl
{
public:
  CPortalManager() {};
  ~CPortalManager() {Done();};
  
  bool Init(const string& _szFileName);
  bool Init(CXMLTreeNode& _xmlLevel);
  
  CRoom*   GetRoom  (const string& _szName);
  CPortal* GetPortal(const string& _szName);

  void Render(CRenderManager* _pRM) {Render(_pRM,false);};
  void DebugRender(CRenderManager* _pRM);

protected:
  virtual void Release() {};
private:
  void Render(CRenderManager* _pRM, bool _bDebug);

  map<string,CRoom>   m_Rooms;
  map<string,CPortal> m_Portals;

  string              m_szCameraLastRoom;

  
};

#endif

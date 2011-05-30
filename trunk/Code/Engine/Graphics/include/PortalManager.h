#pragma once
#ifndef __PORTAL_MANAGER_H__
#define __PORTAL_MANAGER_H__

#include "base.h"
#include "Room.h"
#include "Portal.h"

class CPortalManager:
  public CBaseControl
{
public:
  CPortalManager() {};
  ~CPortalManager() {Done();};

  bool Init(CXMLTreeNode& _xmlLevel);
  
  CRoom*   GetRoom  (const string& _szName);
  CPortal* GetPortal(const string& _szName);

protected:
  virtual void Release() {};
private:

  map<string,CRoom>   m_Rooms;
  map<string,CPortal> m_Portals;
};

#endif

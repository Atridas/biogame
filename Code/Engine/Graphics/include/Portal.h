#pragma once
#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "base.h"

// forward---------------------
class CRoom;
class CXMLTreeNode;
class CPortalManager;
// ----------------------------

class CPortal:
  public CObject3D,
  public CBaseUtils
{
public:
  CPortal():m_pRoomA(0), m_pRoomB(0) {};
  ~CPortal() {Done();};

  bool Init(const CXMLTreeNode&, const CPortalManager*);
  
  CRoom* GetRoomA() const {return m_pRoomA;};
  CRoom* GetRoomB() const {return m_pRoomB;};

protected:
  virtual void Release() {m_pRoomA = m_pRoomB = 0;};
private:
  CRoom* m_pRoomA, m_pRoomB;
};

#endif

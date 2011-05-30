#pragma once
#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "base.h"
#include "Utils\Object3D.h"

// forward---------------------
class CRoom;
class CXMLTreeNode;
class CPortalManager;
// ----------------------------

class CPortal:
  public CObject3D,
  public CBaseControl
{
public:
  CPortal():m_pRoomA(0), m_pRoomB(0) {};
  ~CPortal() {Done();};

  bool Init(CXMLTreeNode&, CPortalManager*);
  
  CRoom* GetRoomA() const {return m_pRoomA;};
  CRoom* GetRoomB() const {return m_pRoomB;};

protected:
  virtual void Release() {m_pRoomA = m_pRoomB = 0;};
private:
  CRoom* m_pRoomA, *m_pRoomB;
};

#endif

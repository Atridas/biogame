#pragma once
#ifndef __ROOM_H__
#define __ROOM_H__

#include "base.h"
#include "Utils\BoundingBox.h"
#include "Utils\Object3D.h"
#include "Named.h"

#include <set>

#include "RenderableObject.h"
#include <queue>

// Forward declarations ------------
class CXMLTreeNode;
class CRenderManager;
class CFrustum;
class CPortal;
// ---------------------------------

class CRoom:
  public CBaseControl,
  public CNamed
{
public:
  typedef priority_queue<CRenderableObject*,vector< CRenderableObject*>, CRenderableObjectOrdering> TBlendQueue;

  CRoom():CNamed("undefined") {};
  ~CRoom() {Done();};

  bool Init(CXMLTreeNode&,set<string>& _UsedGameObjects);

  const vector<CObject3D> GetBoundings() const { return m_Boundings; }

  void Render(CRenderManager* _pRM, const CFrustum& _Frustum, TBlendQueue& _BlendQueue) const;
  void DebugRender(CRenderManager* _pRM) const;

  void AddRendeableObject(CRenderableObject*);
  void RemoveRendeableObject(CRenderableObject*);

  void AddPortal(CPortal* _pPortal) {m_Portals.push_back(_pPortal);};
  const vector<CPortal*>& GetPortals() const { return m_Portals;};

  bool IsObject3DSphereInRoom(const CObject3D&) const;

protected:
  virtual void Release() {};
private:

  vector<CObject3D> m_Boundings;
  vector<CPortal*>  m_Portals;
  set<CRenderableObject*> m_RenderableObjects;
};

#endif

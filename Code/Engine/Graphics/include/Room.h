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
  void AddRendeableObject(CRenderableObject*);
  void RemoveRendeableObject(CRenderableObject*);

  bool IsObject3DInRoom(const CObject3D&);

protected:
  virtual void Release() {};
private:

  vector<CObject3D> m_Boundings;
  set<CRenderableObject*> m_RenderableObjects;
};

#endif

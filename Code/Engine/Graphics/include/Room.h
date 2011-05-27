#pragma once
#ifndef __ROOM_H__
#define __ROOM_H__

#include "base.h"
#include "Utils\BoundingBox.h"
#include "Utils\Object3D.h"

#include <set>

#include "RenderableObject.h"
#include <queue>

// Forward declarations ------------
class CXMLTreeNode;
class CRenderManager;
// ---------------------------------

class CRoom:
  public CBaseControl
{
public:
  CRoom() {};
  ~CRoom() {Done();};

  bool Init(CXMLTreeNode&,set<string>&);

  const vector<CObject3D> GetBoundings() const { return m_Boundings; }
  void Render(CRenderManager*, priority_queue<CRenderableObject*,vector< CRenderableObject*>, CRenderableObjectOrdering>& _BlendQueue) const;
  void AddRendeableObject(CRenderableObject*);
  void RemoveRendeableObject(CRenderableObject*);

protected:
  virtual void Release() {};
private:

  vector<CObject3D> m_Boundings;
  set<CRenderableObject*> m_RenderableObjects;
};

#endif

#pragma once
#ifndef __ROOM_H__
#define __ROOM_H__

#include "base.h"
#include "Utils\BoundingBox.h"
#include "Utils\Object3D.h"

#include <set>

// Forward declarations ------------
class CRenderableObject;
class CXMLTreeNode;
class CRenderManager;
// ---------------------------------

class CRoom:
  public CObject3D,
  public CBaseControl
{
public:
  CRoom() {};
  ~CRoom() {Done();};

  bool Init(const CXMLTreeNode&);

  const vector<CBoundingBox> GetBoundingBoxes() const { return m_BoundingBoxes; }
  void Render(CRenderManager*) const;
  void AddRendeableObject(CRenderableObject*);
  void RemoveRendeableObject(CRenderableObject*);

protected:
  virtual Release() {};
private:

  vector<CBoundingBox> m_BoundingBoxes;
  set<CRenderableObject*> m_RenderableObjects;
}

#endif

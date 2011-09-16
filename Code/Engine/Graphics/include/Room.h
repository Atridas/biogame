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
class CPortalManager;
class CEmiterInstance;
// ---------------------------------

class CRoom:
  public CBaseControl,
  public CNamed
{
public:
  //typedef priority_queue<CRenderableObject*,vector< CRenderableObject*>, CRenderableObjectOrdering> TBlendQueue;
  typedef priority_queue<CObject3DRenderable*,vector< CObject3DRenderable*>, CObject3DOrdering> TBlendQueue;

  CRoom():CNamed("undefined"),m_bRendered(true), m_bNeightbour(true) {};
  ~CRoom() {Done();};
  
  bool Init();
  bool Init(CXMLTreeNode& _xmlNode, set<string>& _UsedNames);

  const vector<CObject3D> GetBoundings() const { return m_Boundings; }
  
  void Render(CRenderManager* _pRM, const CFrustum& _Frustum, TBlendQueue& _BlendQueue, TBlendQueue& _EmiterQueue) const;
  void GetRenderedObjects(const CFrustum& _Frustum, vector<CRenderableObject*>& OpaqueObjects_, TBlendQueue& _BlendQueue, TBlendQueue& _EmiterQueue) const;
  void DebugRender(CRenderManager* _pRM) const;
  void Update(CPortalManager* _pPM);

  void AddRendeableObject(CRenderableObject*);
  bool RemoveRendeableObject(CRenderableObject*);

  void AddEmiter(CEmiterInstance*);
  bool RemoveEmiter(CEmiterInstance*);

  void AddPortal(CPortal* _pPortal) {m_Portals.push_back(_pPortal);};
  const vector<CPortal*>& GetPortals() const { return m_Portals;};

  bool IsObject3DSphereInRoom(const CObject3D*) const;
  
  void SetRendered  (bool _bRendered)   { m_bRendered   = _bRendered  ;}
  void SetNeightbour(bool _bNeightbour) { m_bNeightbour = _bNeightbour;}
  bool GetRendered  ( ) const { return m_bRendered  ;}
  bool GetNeightbour( ) const { return m_bNeightbour;}

protected:
  virtual void Release() {m_Boundings.clear();m_Portals.clear();m_RenderableObjects.clear();m_Emiters.clear();};
private:
  bool m_bRendered, m_bNeightbour;

  vector<CObject3D>       m_Boundings;
  vector<CPortal*>        m_Portals;
  map<CRenderableObject*, Vect3f> m_RenderableObjects;
  map<CEmiterInstance*, Vect3f>   m_Emiters;
};

#endif

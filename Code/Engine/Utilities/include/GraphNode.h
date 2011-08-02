#pragma once
#ifndef __GRAPH_NODE_H__
#define __GRAPH_NODE_H__

#ifndef __GRAPH_DEFINES_H__
#error S'ha d'incloure "GraphDefines.h" i no "GraphNode.h"
#endif

#include "base.h"

// forward declarations
class CGameEntity;
// --------------------

class CGraphNode:
  public CBaseControl
{
public:
  CGraphNode():m_iIndex(INVALID_GRAPH_NODE_INDEX),m_pEntity(0) {SetOk(true);}

  CGraphNode(int _iIndex, const Vect3f& _Pos):m_iIndex(_iIndex),m_vPosition(_Pos)  {SetOk(true);}

  virtual ~CGraphNode() {Done();}

  virtual void Release() {};

  int GetIndex()              const { return m_iIndex;    };
  const Vect3f& GetPosition() const { return m_vPosition; };
  CGameEntity*  GetEntity()   const { return m_pEntity;   };

  void SetIndex   (int _iIndex)              { m_iIndex    = _iIndex;    };
  void SetPosition(const Vect3f& _vPosition) { m_vPosition = _vPosition; };
  void SetEntity  (CGameEntity*  _pEntity)   { m_pEntity   = _pEntity;   };

private:
  int m_iIndex;
  CGameEntity* m_pEntity;

  Vect3f m_vPosition;
};

#endif
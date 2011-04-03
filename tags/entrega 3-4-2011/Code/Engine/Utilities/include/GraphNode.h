#pragma once
#ifndef __GRAPH_NODE_H__
#define __GRAPH_NODE_H__

#ifndef __GRAPH_DEFINES_H__
#error S'ha d'incloure "GraphDefines.h" i no "GraphNode.h"
#endif

#include "base.h"

class CGraphNode:
  public CBaseControl
{
public:
  CGraphNode():m_iIndex(INVALID_GRAPH_NODE_INDEX) {SetOk(true);}

  CGraphNode(int _iIndex, const Vect3f& _Pos):m_iIndex(_iIndex),m_vPosition(_Pos)  {SetOk(true);}

  virtual ~CGraphNode() {Done();}

  virtual void Release() {};

  int GetIndex() const { return m_iIndex; };
  const Vect3f& GetPosition() const { return m_vPosition; };
  
  void SetIndex(int _iIndex) { m_iIndex = _iIndex;};
  void SetPosition(const Vect3f& _vPosition) { m_vPosition = _vPosition;};

private:
  int m_iIndex;

  Vect3f m_vPosition;
};

#endif
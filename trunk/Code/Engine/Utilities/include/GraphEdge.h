#pragma once
#ifndef __GRAPH_EDGE_H__
#define __GRAPH_EDGE_H__

#ifndef __GRAPH_DEFINES_H__
#error S'ha d'incloure "GraphDefines.h" i no "GraphEdge.h"
#endif

#include "base.h"

class CGraphEdge:
  public CBaseControl
{
public:
  CGraphEdge(int _iFrom, int _iTo, float _fCost): 
                m_iFrom(_iFrom),
                m_iTo(_iTo),
                m_fCost(_fCost) 
                {SetOk(true);};
  CGraphEdge(int _iFrom, int _iTo): 
                m_iFrom(_iFrom),
                m_iTo(_iTo),
                m_fCost(1.0f) 
                {SetOk(true);};
  CGraphEdge(): 
                m_iFrom(INVALID_GRAPH_NODE_INDEX),
                m_iTo(INVALID_GRAPH_NODE_INDEX),
                m_fCost(1.0f) 
                {SetOk(true);};

  ~CGraphEdge() {Done();};

  virtual void Release() {};

  

  
  int   GetFrom() const {return m_iFrom;};
  int   GetTo()   const {return m_iTo;};
  float GetCost() const {return m_fCost;};
  
  void SetFrom(int _iFrom)   {m_iFrom = _iFrom;};
  void SetTo(int _iTo)       {m_iTo = _iTo;};
  void SetCost(float _fCost) {m_fCost = _fCost;};

private:
  int m_iFrom;
  int m_iTo;

  float m_fCost;
};

#endif
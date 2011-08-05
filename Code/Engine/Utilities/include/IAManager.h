#pragma once
#ifndef __IA_MANAGER__
#define __IA_MANAGER__

#include "base.h"

// forward declarations
class CSparseGraph;
class CGraphNode;
// --------------------

class CIAManager:
  public CBaseControl
{
public:
  CIAManager():m_pGraph(0) {};
  ~CIAManager() {Done();};

  bool Init();

  CSparseGraph* GetGraph() const {return m_pGraph;};

  void TraceEdges();
  int GetClosestNode(const Vect3f& _vPosition); //Utilitzar aquest, i no el del graf!!!! El del graf no s'optimitzarà!

protected:
  virtual void Release();
private:

  bool EdgeValid(const CGraphNode&, const CGraphNode&);

  CSparseGraph * m_pGraph;
};

#endif
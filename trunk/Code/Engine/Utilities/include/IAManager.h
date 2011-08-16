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

  void CompleteGraph();
  int GetClosestNode(const Vect3f& _vPosition); //Utilitzar aquest, i no el del graf!!!! El del graf no s'optimitzarà!
  vector<CGraphNode*> GetClosestCobertura   (const Vect3f& _vPosition);
  vector<Vect3f>      GetClosestCoberturaVec(const Vect3f& _vPosition);
  
  vector<CGraphNode*> SearchPath   (const Vect3f& _vOrigin, const Vect3f& _vDestination);
  vector<Vect3f>      SearchPathVec(const Vect3f& _vOrigin, const Vect3f& _vDestination);

protected:
  virtual void Release();
private:
  
  void TraceEdges();
  bool EdgeValid(const CGraphNode&, const CGraphNode&);

  CSparseGraph * m_pGraph;

  set<int> m_NodesCobertura;
};

#endif
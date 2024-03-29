#pragma once
#ifndef __SEARCH_A_STAR_H__
#define __SEARCH_A_STAR_H__

#include "GraphDefines.h"

class CHeuristic;

using namespace std;
//------------------------------- Graph_SearchAStar --------------------------
//
//  this searchs a graph using the distance between the target node and the 
//  currently considered node as a heuristic.
//
//  This search is more commonly known as A* (pronounced Ay-Star)
//-----------------------------------------------------------------------------
class CSearchAStar
{
public:
  
  CSearchAStar(const CSparseGraph& graph,
               const CHeuristic*   heuristic,
                      int          source,
                      int          target):m_Graph(graph),
                                           m_pHeuristic(heuristic),
                                           m_ShortestPathTree(graph.NumNodes()),                              
                                           m_SearchFrontier(graph.NumNodes()),
                                           m_GCosts(graph.NumNodes(), 0.0),
                                           m_FCosts(graph.NumNodes(), 0.0),
                                           m_iSource(source)
  {
    m_Targets.insert(target);
    Search();   
  }


  CSearchAStar(const CSparseGraph&     graph,
               const CHeuristic*       heuristic,
                      int              source,
                      const set<int>&  targets):m_Graph(graph),
                                                m_pHeuristic(heuristic),
                                                m_ShortestPathTree(graph.NumNodes()),                              
                                                m_SearchFrontier(graph.NumNodes()),
                                                m_GCosts(graph.NumNodes(), 0.0),
                                                m_FCosts(graph.NumNodes(), 0.0),
                                                m_iSource(source)
  {
    set<int>::iterator l_it  = targets.begin();
    set<int>::iterator l_end = targets.end();
    for(; l_it != l_end; ++l_it) m_Targets.insert(*l_it);
    Search();   
  }
 
  //returns the vector of edges that the algorithm has examined
  std::vector<const CGraphEdge*> GetSPT()const{return m_ShortestPathTree;}

  //returns a vector of node indexes that comprise the shortest path
  //from the source to the target
  std::list<int> GetPathToTarget()const;

  //returns the total cost to the target
  double GetCostToTarget()const{return m_GCosts[m_iTargetFinal];}


private:

  const CSparseGraph&              m_Graph;

  const CHeuristic*                m_pHeuristic;

  //indexed into my node. Contains the 'real' accumulative cost to that node
  std::vector<float>               m_GCosts; 

  //indexed into by node. Contains the cost from adding m_GCosts[n] to
  //the heuristic cost from n to the target node. This is the vector the
  //iPQ indexes into.
  std::vector<float>               m_FCosts;

  std::vector<const CGraphEdge*>  m_ShortestPathTree;
  std::vector<const CGraphEdge*>  m_SearchFrontier;

  int                            m_iSource;
  std::set<int>                  m_Targets;

  int                            m_iTargetFinal;

  //the A* search algorithm
  void Search();
};

#endif
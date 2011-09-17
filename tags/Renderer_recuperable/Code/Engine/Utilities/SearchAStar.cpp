#include "SearchAStar.h"

#include "Utils/PriorityQueue.h"
#include "Heuristics.h"

//-----------------------------------------------------------------------------
void CSearchAStar::Search()
{
  //create an indexed priority queue of nodes. The nodes with the
  //lowest overall F cost (G+H) are positioned at the front.
  IndexedPriorityQLow<float> pq(m_FCosts, m_Graph.NumNodes());

  //put the source node on the queue
  pq.insert(m_iSource);

  //while the queue is not empty
  while(!pq.empty())
  {
    //get lowest cost node from the queue
    int NextClosestNode = pq.Pop();

    //move this node from the frontier to the spanning tree
    m_ShortestPathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode];

    //if the target has been found exit
    if (m_Targets.find(NextClosestNode) != m_Targets.end())
    {
      m_iTargetFinal = NextClosestNode;
      return;
    }

    //now to test all the edges attached to this node
    CSparseGraph::ConstEdgeIterator ConstEdgeItr(m_Graph, NextClosestNode);

    for (const CGraphEdge* pE=ConstEdgeItr.begin();
        !ConstEdgeItr.end(); 
         pE=ConstEdgeItr.next())
    {
      //calculate the heuristic cost from this node to the target (H)         
      set<int>::iterator l_it  = m_Targets.begin();
      set<int>::iterator l_end = m_Targets.end();
      float HCost = m_pHeuristic->Calculate(m_Graph, *l_it, pE->GetTo()); 

      for(++l_it; l_it != l_end; ++l_it) //saltem el primer, que ja l'hem fet!
      {
        float l_fNewHeuristic = m_pHeuristic->Calculate(m_Graph, *l_it, pE->GetTo());
        if(l_fNewHeuristic < HCost)
        {
          HCost = l_fNewHeuristic;
        }
      }

      //calculate the 'real' cost to this node from the source (G)
      float GCost = m_GCosts[NextClosestNode] + pE->GetCost();

      //if the node has not been added to the frontier, add it and update
      //the G and F costs
      if (m_SearchFrontier[pE->GetTo()] == NULL)
      {
        m_FCosts[pE->GetTo()] = GCost + HCost;
        m_GCosts[pE->GetTo()] = GCost;

        pq.insert(pE->GetTo());

        m_SearchFrontier[pE->GetTo()] = pE;
      }

      //if this node is already on the frontier but the cost to get here
      //is cheaper than has been found previously, update the node
      //costs and frontier accordingly.
      else if ((GCost < m_GCosts[pE->GetTo()]) && (m_ShortestPathTree[pE->GetTo()]==NULL))
      {
        m_FCosts[pE->GetTo()] = GCost + HCost;
        m_GCosts[pE->GetTo()] = GCost;

        pq.ChangePriority(pE->GetTo());

        m_SearchFrontier[pE->GetTo()] = pE;
      }
    }
  }
}

//-----------------------------------------------------------------------------
std::list<int> CSearchAStar::GetPathToTarget()const
{
  std::list<int> path;

  //just return an empty path if no target or no path found
  if (m_iTargetFinal < 0)  return path;    

  int nd = m_iTargetFinal;

  path.push_front(nd);
    
  while ((nd != m_iSource) && (m_ShortestPathTree[nd] != 0))
  {
    nd = m_ShortestPathTree[nd]->GetFrom();

    path.push_front(nd);
  }

  return path;
} 
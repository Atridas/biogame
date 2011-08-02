#pragma once
#ifndef __SPARSE_GRAPH_H__
#define __SPARSE_GRAPH_H__

#ifndef __GRAPH_DEFINES_H__
#error S'ha d'incloure "GraphDefines.h" i no "SparseGraph.h"
#endif

#include "base.h"
#include <list>

class CRenderManager;

class CSparseGraph                                 
{
public:
  
  //a couple more typedefs to save my fingers and to help with the formatting
  //of the code on the printed page
  typedef std::vector<CGraphNode>   NodeVector;
  typedef std::list<CGraphEdge>     EdgeList;
  typedef std::vector<EdgeList>     EdgeListVector;

 
private:
  
  //the nodes that comprise this graph
  NodeVector      m_Nodes;

  //a vector of adjacency edge lists. (each node index keys into the 
  //list of edges associated with that node)
  EdgeListVector  m_Edges;
 
  //is this a directed graph?
  bool            m_bDigraph;

  //the index of the next node to be added
  int             m_iNextNodeIndex;
  
    
  //returns true if an edge is not already present in the graph. Used
  //when adding edges to make sure no duplicates are created.
  bool  UniqueEdge(int from, int to)const;

  //iterates through all the edges in the graph and removes any that point
  //to an invalidated node
  void  CullInvalidEdges();
  
public:
  
  //ctor
  CSparseGraph(bool digraph): m_iNextNodeIndex(0), m_bDigraph(digraph){}


  void DebugRender(CRenderManager* _pRM) const;

  //returns the node at the given index
  const CGraphNode&  GetNode(int idx)const;

  //non const version
  CGraphNode&  GetNode(int idx);

  int GetClosestNode(const Vect3f& _vPosition);

  //const method for obtaining a reference to an edge
  const CGraphEdge& GetEdge(int from, int to)const;

  //non const version
  CGraphEdge& GetEdge(int from, int to);
    

  //retrieves the next free node index
  int   GetNextFreeNodeIndex()const{return m_iNextNodeIndex;}
  
  //adds a node to the graph and returns its index
  int   AddNode(const CGraphNode& node);

  //removes a node by setting its index to invalid_node_index
  void  RemoveNode(int node);

  //Use this to add an edge to the graph. The method will ensure that the
  //edge passed as a parameter is valid before adding it to the graph. If the
  //graph is a digraph then a similar edge connecting the nodes in the opposite
  //direction will be automatically added.
  void  AddEdge(const CGraphEdge& edge);

  //removes the edge connecting from and to from the graph (if present). If
  //a digraph then the edge connecting the nodes in the opposite direction 
  //will also be removed.
  void  RemoveEdge(int from, int to);

  //sets the cost of an edge
  void  SetEdgeCost(int from, int to, float cost);

  //returns the number of active + inactive nodes present in the graph
  int   NumNodes()const{return m_Nodes.size();}
  
  //returns the number of active nodes present in the graph (this method's
  //performance can be improved greatly by caching the value)
  int   NumActiveNodes()const
  {
    int count = 0;

    for (unsigned int n=0; n<m_Nodes.size(); ++n) if (m_Nodes[n].GetIndex() != INVALID_GRAPH_NODE_INDEX) ++count;

    return count;
  }

  //returns the total number of edges present in the graph
  int   NumEdges()const
  {
    int tot = 0;

    for (EdgeListVector::const_iterator curEdge = m_Edges.begin();
         curEdge != m_Edges.end();
         ++curEdge)
    {
      tot += curEdge->size();
    }

    return tot;
  }

  //returns true if the graph is directed
  bool  isDigraph()const{return m_bDigraph;}

  //returns true if the graph contains no nodes
  bool	isEmpty()const{return m_Nodes.empty();}

  //returns true if a node with the given index is present in the graph
  bool isNodePresent(int nd)const;

  //returns true if an edge connecting the nodes 'to' and 'from'
  //is present in the graph
  bool isEdgePresent(int from, int to)const;

  //methods for loading and saving graphs from an open file stream or from
  //a file name 
  //bool  Save(const char* FileName)const;
  //bool  Save(std::ofstream& stream)const;

  //bool  Load(const char* FileName);
  //bool  Load(std::ifstream& stream);

  //clears the graph ready for new node insertions
  void Clear(){m_iNextNodeIndex = 0; m_Nodes.clear(); m_Edges.clear();}

  void RemoveEdges()
  {
    for (EdgeListVector::iterator it = m_Edges.begin(); it != m_Edges.end(); ++it)
    {
      it->clear();
    }
  }

  
    //non const class used to iterate through all the edges connected to a specific node. 
      class EdgeIterator
      {
      private:                                                                

        EdgeList::iterator curEdge;
        CSparseGraph&      G;
        const int          NodeIndex;

      public:

        EdgeIterator(CSparseGraph& graph,
                     int          node): G(graph),
                                         NodeIndex(node)
        {
          /* we don't need to check for an invalid node index since if the node is
             invalid there will be no associated edges
         */

          curEdge = G.m_Edges[NodeIndex].begin();
        }

        CGraphEdge*  begin()
        {        
          curEdge = G.m_Edges[NodeIndex].begin();
    
          if(end()) return 0;
          return &(*curEdge);
        }

        CGraphEdge*  next()
        {
          ++curEdge;
    
          if(end()) return 0;
          return &(*curEdge);

        }

        //return true if we are at the end of the edge list
        bool end()
        {
          return (curEdge == G.m_Edges[NodeIndex].end());
        }
      };

  friend class EdgeIterator;

  //const class used to iterate through all the edges connected to a specific node. 
      class ConstEdgeIterator
      {
      private:                                                                

        EdgeList::const_iterator curEdge;
        const CSparseGraph&      G;
        const int                NodeIndex;

      public:

        ConstEdgeIterator(const CSparseGraph& graph,
                          int                 node): G(graph),
                                                     NodeIndex(node)
        {
          /* we don't need to check for an invalid node index since if the node is
             invalid there will be no associated edges
         */

          curEdge = G.m_Edges[NodeIndex].begin();
        }

        const CGraphEdge*  begin()
        {        
          curEdge = G.m_Edges[NodeIndex].begin();
    
          if(end()) return 0;
          return &(*curEdge);
        }

        const CGraphEdge*  next()
        {
          ++curEdge;
    
          if(end()) return 0;
          return &(*curEdge);

        }

        //return true if we are at the end of the edge list
        bool end()
        {
          return (curEdge == G.m_Edges[NodeIndex].end());
        }
      };

  friend class ConstEdgeIterator;

  //non const class used to iterate through the nodes in the graph
    class NodeIterator
    {
    private:

      NodeVector::iterator curNode;
      CSparseGraph&        G;

      //if a graph node is removed, it is not removed from the 
      //vector of nodes (because that would mean changing all the indices of 
      //all the nodes that have a higher index). This method takes a node
      //iterator as a parameter and assigns the next valid element to it.
      void GetNextValidNode(NodeVector::iterator& it)
      {
        if ( curNode == G.m_Nodes.end() || it->GetIndex() != INVALID_GRAPH_NODE_INDEX) return;

        while ( (it->GetIndex() == INVALID_GRAPH_NODE_INDEX) )
        {
          ++it;

          if (curNode == G.m_Nodes.end()) break;
        }
      }

    public:
      
      NodeIterator(CSparseGraph &graph):G(graph)
      {
        curNode = G.m_Nodes.begin();
      }


      CGraphNode* begin()
      {      
        curNode = G.m_Nodes.begin();

        GetNextValidNode(curNode);
        
        if(end()) return 0;
        return &(*curNode);
      }

      CGraphNode* next()
      {
        ++curNode;

        GetNextValidNode(curNode);
        
        if(end()) return 0;
        return &(*curNode);
      }

      bool end()
      {
        return (curNode == G.m_Nodes.end());
      }
    };

     
  friend class NodeIterator;

    //const class used to iterate through the nodes in the graph
    class ConstNodeIterator
    {
    private:

      NodeVector::const_iterator curNode;
      const CSparseGraph&        G;

      //if a graph node is removed or switched off, it is not removed from the 
      //vector of nodes (because that would mean changing all the indices of 
      //all the nodes that have a higher index. This method takes a node
      //iterator as a parameter and assigns the next valid element to it.
      void GetNextValidNode(NodeVector::const_iterator& it)
      {
        if ( curNode == G.m_Nodes.end() || it->GetIndex() != INVALID_GRAPH_NODE_INDEX) return;

        while ( (it->GetIndex() == INVALID_GRAPH_NODE_INDEX) )
        {
          ++it;

          if (curNode == G.m_Nodes.end()) break;
        }
      }

    public:

      ConstNodeIterator(const CSparseGraph &graph):G(graph)
      {
        curNode = G.m_Nodes.begin();
      }


      const CGraphNode* begin()
      {      
        curNode = G.m_Nodes.begin();

        GetNextValidNode(curNode);

        if(end()) return 0;
        return &(*curNode);
      }

      const CGraphNode* next()
      {
        ++curNode;

        GetNextValidNode(curNode);
        
        if(end()) return 0;
        return &(*curNode);
      }

      bool end()
      {
        return (curNode == G.m_Nodes.end());
      }
    };

  friend class ConstNodeIterator;
};



#endif
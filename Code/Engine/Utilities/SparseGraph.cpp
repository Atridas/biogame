#include "GraphDefines.h"

#include "RenderManager.h"

//--------------------------- isNodePresent --------------------------------
//
//  returns true if a node with the given index is present in the graph
//--------------------------------------------------------------------------
bool CSparseGraph::isNodePresent(int nd)const
{
    if ( (m_Nodes[nd].GetIndex() == INVALID_GRAPH_NODE_INDEX) || (nd >= (int)m_Nodes.size()))
    {
      return false;
    }
    else return true;
}

//--------------------------- isEdgePresent --------------------------------
//
//  returns true if an edge with the given from/to is present in the graph
//--------------------------------------------------------------------------
bool CSparseGraph::isEdgePresent(int from, int to)const
{
    if (isNodePresent(from) && isNodePresent(from))
    {
       for (EdgeList::const_iterator curEdge = m_Edges[from].begin();
            curEdge != m_Edges[from].end();
            ++curEdge)
        {
          if (curEdge->GetTo() == to) return true;
        }

        return false;
    }
    else return false;
}
//------------------------------ GetNode -------------------------------------
//
//  const and non const methods for obtaining a reference to a specific node
//----------------------------------------------------------------------------
const CGraphNode&  CSparseGraph::GetNode(int idx)const
{
    assert( (idx < (int)m_Nodes.size()) &&
            (idx >=0)              &&
           "<SparseGraph::GetNode>: invalid index");

    return m_Nodes[idx];
}

  //non const version
CGraphNode&  CSparseGraph::GetNode(int idx)
{
    assert( (idx < (int)m_Nodes.size()) &&
            (idx >=0)             &&
          "<SparseGraph::GetNode>: invalid index");
    
    return m_Nodes[idx];
}

//------------------------------ GetEdge -------------------------------------
//
//  const and non const methods for obtaining a reference to a specific edge
//----------------------------------------------------------------------------
const CGraphEdge& CSparseGraph::GetEdge(int from, int to)const
{
  assert( (from < (int)m_Nodes.size()) &&
          (from >=0)              &&
           (int)m_Nodes[from].GetIndex() != INVALID_GRAPH_NODE_INDEX &&
          "<SparseGraph::GetEdge>: invalid 'from' index");

  assert( (to < (int)m_Nodes.size()) &&
          (to >=0)              &&
          (int)m_Nodes[to].GetIndex() != INVALID_GRAPH_NODE_INDEX &&
          "<SparseGraph::GetEdge>: invalid 'to' index");

  for (EdgeList::const_iterator curEdge = m_Edges[from].begin();
       curEdge != m_Edges[from].end();
       ++curEdge)
  {
    if (curEdge->GetTo() == to) return *curEdge;
  }

  assert (0 && "<SparseGraph::GetEdge>: edge does not exist");
  return *(m_Edges[0].begin());
}

//non const version
CGraphEdge& CSparseGraph::GetEdge(int from, int to)
{
  assert( (from < (int)m_Nodes.size()) &&
          (from >=0)              &&
           (int)m_Nodes[from].GetIndex() != INVALID_GRAPH_NODE_INDEX &&
          "<SparseGraph::GetEdge>: invalid 'from' index");

  assert( (to < (int)m_Nodes.size()) &&
          (to >=0)              &&
          (int)m_Nodes[to].GetIndex() != INVALID_GRAPH_NODE_INDEX &&
          "<SparseGraph::GetEdge>: invalid 'to' index");

  for (EdgeList::iterator curEdge = m_Edges[from].begin();
       curEdge != m_Edges[from].end();
       ++curEdge)
  {
    if (curEdge->GetTo() == to) return *curEdge;
  }

  assert (0 && "<SparseGraph::GetEdge>: edge does not exist");
  return *(m_Edges[0].begin());
}

//-------------------------- AddEdge ------------------------------------------
//
//  Use this to add an edge to the graph. The method will ensure that the
//  edge passed as a parameter is valid before adding it to the graph. If the
//  graph is a digraph then a similar edge connecting the nodes in the opposite
//  direction will be automatically added.
//-----------------------------------------------------------------------------
void CSparseGraph::AddEdge(const CGraphEdge& edge)
{
  //first make sure the from and to nodes exist within the graph 
  assert( (edge.GetFrom() < m_iNextNodeIndex) && (edge.GetTo() < m_iNextNodeIndex) &&
          "<SparseGraph::AddEdge>: invalid node index");

  //make sure both nodes are active before adding the edge
  if ( (m_Nodes[edge.GetTo()].GetIndex() != INVALID_GRAPH_NODE_INDEX) && 
       (m_Nodes[edge.GetFrom()].GetIndex() != INVALID_GRAPH_NODE_INDEX))
  {
    //add the edge, first making sure it is unique
    if (UniqueEdge(edge.GetFrom(), edge.GetTo()))
    {
      m_Edges[edge.GetFrom()].push_back(edge);
    }

    //if the graph is undirected we must add another connection in the opposite
    //direction
    if (!m_bDigraph)
    {
      //check to make sure the edge is unique before adding
      if (UniqueEdge(edge.GetTo(), edge.GetFrom()))
      {
        CGraphEdge NewEdge = edge;

        NewEdge.SetTo(edge.GetFrom());
        NewEdge.SetFrom(edge.GetTo());

        m_Edges[edge.GetTo()].push_back(NewEdge);
      }
    }
  }
}


//----------------------------- RemoveEdge ---------------------------------
void CSparseGraph::RemoveEdge(int from, int to)
{
  assert ( (from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) &&
           "<SparseGraph::RemoveEdge>:invalid node index");

  EdgeList::iterator curEdge;
  
  if (!m_bDigraph)
  {
    for (curEdge = m_Edges[to].begin();
         curEdge != m_Edges[to].end();
         ++curEdge)
    {
      if (curEdge->GetTo() == from){curEdge = m_Edges[to].erase(curEdge);break;}
    }
  }

  for (curEdge = m_Edges[from].begin();
       curEdge != m_Edges[from].end();
       ++curEdge)
  {
    if (curEdge->GetTo() == to){curEdge = m_Edges[from].erase(curEdge);break;}
  }
}

//-------------------------- AddNode -------------------------------------
//
//  Given a node this method first checks to see if the node has been added
//  previously but is now innactive. If it is, it is reactivated.
//
//  If the node has not been added previously, it is checked to make sure its
//  index matches the next node index before being added to the graph
//------------------------------------------------------------------------
int CSparseGraph::AddNode(const CGraphNode& node)
{
  if (node.GetIndex() < (int)m_Nodes.size())
  {
    //make sure the client is not trying to add a node with the same ID as
    //a currently active node
    assert (m_Nodes[node.GetIndex()].GetIndex() == INVALID_GRAPH_NODE_INDEX &&
      "<SparseGraph::AddNode>: Attempting to add a node with a duplicate ID");
    
    m_Nodes[node.GetIndex()] = node;

    return m_iNextNodeIndex;
  }
  
  else
  {
    //make sure the new node has been indexed correctly
    assert (node.GetIndex() == m_iNextNodeIndex && "<SparseGraph::AddNode>:invalid index");

    m_Nodes.push_back(node);
    m_Edges.push_back(EdgeList());

    return m_iNextNodeIndex++;
  }
}

//----------------------- CullInvalidEdges ------------------------------------
//
//  iterates through all the edges in the graph and removes any that point
//  to an invalidated node
//-----------------------------------------------------------------------------
void CSparseGraph::CullInvalidEdges()
{
  for (EdgeListVector::iterator curEdgeList = m_Edges.begin(); curEdgeList != m_Edges.end(); ++curEdgeList)
  {
    for (EdgeList::iterator curEdge = (*curEdgeList).begin(); curEdge != (*curEdgeList).end(); ++curEdge)
    {
      if (m_Nodes[curEdge->GetTo()].GetIndex() == INVALID_GRAPH_NODE_INDEX || 
          m_Nodes[curEdge->GetFrom()].GetIndex() == INVALID_GRAPH_NODE_INDEX)
      {
        curEdge = (*curEdgeList).erase(curEdge);
      }
    }
  }
}

  
//------------------------------- RemoveNode -----------------------------
//
//  Removes a node from the graph and removes any links to neighbouring
//  nodes
//------------------------------------------------------------------------
void CSparseGraph::RemoveNode(int node)                                   
{
  assert(node < (int)m_Nodes.size() && "<SparseGraph::RemoveNode>: invalid node index");

  //set this node's index to INVALID_GRAPH_NODE_INDEX
  m_Nodes[node].SetIndex(INVALID_GRAPH_NODE_INDEX);

  //if the graph is not directed remove all edges leading to this node and then
  //clear the edges leading from the node
  if (!m_bDigraph)
  {    
    //visit each neighbour and erase any edges leading to this node
    for (EdgeList::iterator curEdge = m_Edges[node].begin(); 
         curEdge != m_Edges[node].end();
         ++curEdge)
    {
      for (EdgeList::iterator curE = m_Edges[curEdge->GetTo()].begin();
           curE != m_Edges[curEdge->GetTo()].end();
           ++curE)
      {
         if (curE->GetTo() == node)
         {
           curE = m_Edges[curEdge->GetTo()].erase(curE);

           break;
         }
      }
    }

    //finally, clear this node's edges
    m_Edges[node].clear();
  }

  //if a digraph remove the edges the slow way
  else
  {
    CullInvalidEdges();
  }
}

//-------------------------- SetEdgeCost ---------------------------------
//
//  Sets the cost of a specific edge
//------------------------------------------------------------------------
void CSparseGraph::SetEdgeCost(int from, int to, float NewCost)
{
  //make sure the nodes given are valid
  assert( (from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) &&
        "<SparseGraph::SetEdgeCost>: invalid index");

  //visit each neighbour and erase any edges leading to this node
  for (EdgeList::iterator curEdge = m_Edges[from].begin(); 
       curEdge != m_Edges[from].end();
       ++curEdge)
  {
    if (curEdge->GetTo() == to)
    {
      curEdge->SetCost(NewCost);
      break;
    }
  }
}

  //-------------------------------- UniqueEdge ----------------------------
//
//  returns true if the edge is not present in the graph. Used when adding
//  edges to prevent duplication
//------------------------------------------------------------------------
bool CSparseGraph::UniqueEdge(int from, int to)const
{
  for (EdgeList::const_iterator curEdge = m_Edges[from].begin();
       curEdge != m_Edges[from].end();
       ++curEdge)
  {
    if (curEdge->GetTo() == to)
    {
      return false;
    }
  }

  return true;
}
//
////-------------------------------- Save ---------------------------------------
//
//bool CSparseGraph::Save(const char* FileName)const
//{
//  //open the file and make sure it's valid
//  std::ofstream out(FileName);
//
//  if (!out)
//  {
//    throw std::runtime_error("Cannot open file: " + std::string(FileName));
//    return false;
//  }
//
//  return Save(out);
//}
//
////-------------------------------- Save ---------------------------------------
//bool CSparseGraph::Save(std::ofstream& stream)const
//{
//  //save the number of nodes
//  stream << m_Nodes.size() << std::endl;
//
//  //iterate through the graph nodes and save them
//  NodeVector::const_iterator curNode = m_Nodes.begin();
//  for (curNode; curNode!=m_Nodes.end(); ++curNode)
//  {
//    stream << *curNode;
//  }
//
//  //save the number of edges
//  stream << NumEdges() << std::endl;
//
//
//  //iterate through the edges and save them
//  for (unsigned int nodeIdx = 0; nodeIdx < m_Nodes.size(); ++nodeIdx)
//  {
//    for (EdgeList::const_iterator curEdge = m_Edges[nodeIdx].begin();
//         curEdge!=m_Edges[nodeIdx].end(); ++curEdge)
//    {
//      stream << *curEdge;
//    }  
//  }
//
//  return true;
//}
//
////------------------------------- Load ----------------------------------------
////-----------------------------------------------------------------------------
//bool CSparseGraph::Load(const char* FileName)
//{
//  //open file and make sure it's valid
//  std::ifstream in(FileName);
//
//  if (!in)
//  {
//    throw std::runtime_error("Cannot open file: " + std::string(FileName));
//    return false;
//  }
//
//  return Load(in);
//}
//
////------------------------------- Load ----------------------------------------
////-----------------------------------------------------------------------------
//bool CSparseGraph::Load(std::ifstream& stream)
//{
//  Clear();
//
//  //get the number of nodes and read them in
//  int NumNodes, NumEdges;
//
//  stream >> NumNodes;
//
//  for (int n=0; n<NumNodes; ++n)
//  {
//    CGraphNode NewNode(stream);
//  
//    //when editing graphs it's possible to end up with a situation where some
//    //of the nodes have been invalidated (their id's set to INVALID_GRAPH_NODE_INDEX). Therefore
//    //when a node of index INVALID_GRAPH_NODE_INDEX is encountered, it must still be added.
//    if (NewNode.GetIndex() != INVALID_GRAPH_NODE_INDEX)
//    {
//      AddNode(NewNode);
//    }
//    else
//    {
//      m_Nodes.push_back(NewNode);
//
//      //make sure an edgelist is added for each node
//      m_Edges.push_back(EdgeList());
//      
//      ++m_iNextNodeIndex;
//    }
//  }
//
//  //now add the edges
//  stream >> NumEdges;
//  for (int e=0; e<NumEdges; ++e)
//  {
//    CGraphEdge NextEdge(stream);
//
//    m_Edges[NextEdge.GetFrom()].push_back(NextEdge);
//  }
//
//  return true;
//}
//   




int CSparseGraph::GetClosestNode(const Vect3f& _vPosition)
{
  //TODO optimitzar amb algun quad-tree o algo
  int l_iNode = INVALID_GRAPH_NODE_INDEX;
  float l_fDistanceSquared;

  for(uint32 i = 0; i < m_Nodes.size(); ++i)
  {
    float l_fNewDist = m_Nodes[i].GetPosition().SqDistance(_vPosition);
    if(l_iNode == INVALID_GRAPH_NODE_INDEX || l_fDistanceSquared > l_fNewDist)
    {
      l_iNode = i;
      l_fDistanceSquared = l_fNewDist;
    }
  }
  return l_iNode;
}

int CSparseGraph::GetClosestNode(const Vect3f& _vPosition, int _iMaxDistance)
{
  //TODO optimitzar amb algun quad-tree o algo
  int l_iNode = INVALID_GRAPH_NODE_INDEX;
  float l_fDistanceSquared;

  for(uint32 i = 0; i < m_Nodes.size(); ++i)
  {
    float l_fNewDist = m_Nodes[i].GetPosition().SqDistance(_vPosition);
    if((l_iNode == INVALID_GRAPH_NODE_INDEX || l_fDistanceSquared > l_fNewDist) && l_fNewDist <= _iMaxDistance)
    {
      l_iNode = i;
      l_fDistanceSquared = l_fNewDist;
    }
  }
  return l_iNode;
}



// Debug render : ------------------------------------------------------------------------------------------------------

void CSparseGraph::DebugRender(CRenderManager* _pRM) const
{
  Mat44f l_Transform;
  //Renderitzar nodes
  NodeVector::const_iterator l_itNode = m_Nodes.begin();
  for (l_itNode; l_itNode!=m_Nodes.end(); ++l_itNode)
  {
    l_Transform.SetIdentity();
    l_Transform.Translate(l_itNode->GetPosition());

    _pRM->SetTransform(l_Transform);

    _pRM->DrawAxis();
  }
  
  l_Transform.SetIdentity();
  _pRM->SetTransform(l_Transform);

  for (unsigned int nodeIdx = 0; nodeIdx < m_Nodes.size(); ++nodeIdx)
  {
    for (EdgeList::const_iterator curEdge = m_Edges[nodeIdx].begin();
         curEdge!=m_Edges[nodeIdx].end(); ++curEdge)
    {
      _pRM->DrawLine( GetNode( curEdge->GetFrom() ).GetPosition(), 
                      GetNode( curEdge->GetTo()   ).GetPosition(),
                      colCYAN);
    }  
  }
}
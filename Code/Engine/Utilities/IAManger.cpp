#include "IAManager.h"
#include "GraphDefines.h"
#include "EntityDefines.h"
#include "ComponentNavNode.h"
#include "Core.h"
#include "PhysicsManager.h"
#include "SearchAStar.h"
#include "Heuristics.h"


bool CIAManager::Init()
{
  m_pGraph = new CSparseGraph(true);

  SetOk(true);
  return IsOk();
}

void CIAManager::Release()
{
  CHECKED_DELETE(m_pGraph);
}


void CIAManager::TraceEdges()
{
  assert(IsOk());
  CEntityManager *l_pEM = CORE->GetEntityManager();

  int l_iNumNodes = m_pGraph->NumNodes();
  for(int i = 0; i < l_iNumNodes; ++i)
  {
    CGraphNode l_Node = m_pGraph->GetNode(i);
    CGameEntity *l_pNodeEntity = l_Node.GetEntity();
    if(l_pNodeEntity)
    {
      CComponentNavNode *l_pCNN = l_pNodeEntity->GetComponent<CComponentNavNode>();
      if(l_pCNN)
      {
        if(l_pCNN->m_bAutoroute)
        {
          for(int j = i+1; j < l_iNumNodes; ++j)
          {
            CGraphNode l_Node2 = m_pGraph->GetNode(j);
            CGameEntity *l_pNodeEntity2 = l_Node2.GetEntity();
            if(l_pNodeEntity2)
            {
              CComponentNavNode *l_pCNN2 = l_pNodeEntity2->GetComponent<CComponentNavNode>();
              if(l_pCNN2 && l_pCNN2->m_bAutoroute)
              {
                float l_fDistance = l_Node.GetPosition().Distance(l_Node2.GetPosition());
                if((l_pCNN->m_fMaxAutoDistance  < 0 || l_pCNN->m_fMaxAutoDistance  > l_fDistance) && 
                   (l_pCNN2->m_fMaxAutoDistance < 0 || l_pCNN2->m_fMaxAutoDistance > l_fDistance) && 
                    EdgeValid(l_Node, l_Node2))
                {
                  m_pGraph->AddEdge(CGraphEdge(i,j,l_fDistance));
                }
              }
            }
          }
        }
        set<string>::const_iterator l_it  = l_pCNN->m_DefaultEdges.cbegin();
        set<string>::const_iterator l_end = l_pCNN->m_DefaultEdges.cend();
        for(; l_it != l_end; ++l_it)
        {
          CGameEntity *l_pNodeEntity2 = l_pEM->GetEntity(*l_it);
          if(l_pNodeEntity2)
          {
            CComponentNavNode *l_pCNN2 = l_pNodeEntity2->GetComponent<CComponentNavNode>();
            if(l_pCNN2)
            {
              int l_iNodeIndex = l_pCNN2->GetGraphNodeIndex();
              CGraphNode l_Node2 = m_pGraph->GetNode(l_iNodeIndex);
              float l_fDistance = l_Node.GetPosition().Distance(l_Node2.GetPosition());
              m_pGraph->AddEdge(CGraphEdge(i,l_iNodeIndex,l_fDistance));
            }
          }
        }
      }
    }
  }
}

void CIAManager::CompleteGraph()
{
  assert(IsOk());
  TraceEdges();

  m_NodesCobertura.clear();

  CEntityManager *l_pEM = CORE->GetEntityManager();

  int l_iNumNodes = m_pGraph->NumNodes();
  for(int i = 0; i < l_iNumNodes; ++i)
  {
    CGraphNode l_Node = m_pGraph->GetNode(i);
    CGameEntity *l_pNodeEntity = l_Node.GetEntity();
    if(l_pNodeEntity)
    {
      CComponentNavNode *l_pCNN = l_pNodeEntity->GetComponent<CComponentNavNode>();
      if(l_pCNN && l_pCNN->m_bCoberturaBaixa)
      {
        m_NodesCobertura.insert(i); //El node de la cobertura
      }
    }
  }
}

bool CIAManager::EdgeValid(const CGraphNode& _Node1, const CGraphNode& _Node2)
{
  if(_Node1.GetIndex() == _Node2.GetIndex())
  {
    return false;
  }
  
  CPhysicsManager *l_pPM = CORE->GetPhysicsManager();

  SCollisionInfo l_CollisionInfo;
  Vect3f l_Pos = _Node1.GetPosition();
  Vect3f l_Dir = _Node2.GetPosition() - l_Pos;
  l_Dir.Normalize();

  if(!l_pPM->RaycastClosestActor(l_Pos, l_Dir, l_pPM->GetCollisionMask(ECG_RAY_IA_GRAPH), l_CollisionInfo)
    || l_CollisionInfo.m_fDistance > l_Pos.Distance(_Node2.GetPosition()))
  {
    return true;
  }
  else
  {
    return false;
  }
}

int CIAManager::GetClosestNode(const Vect3f& _vPosition)
{
  //TODO optimitzacions
  return m_pGraph->GetClosestNode(_vPosition);
}


vector<CGraphNode*> CIAManager::SearchPath   (const Vect3f& _vOrigin, const Vect3f& _vDestination)
{
  vector<CGraphNode*> Path_;

  int l_iOrigin      = GetClosestNode(_vOrigin);
  int l_iDestination = GetClosestNode(_vDestination);

  if(l_iOrigin == l_iDestination)
  {
    Path_.push_back( &(m_pGraph->GetNode(l_iDestination)) );
    return Path_;
  }


  CSearchAStar search( *m_pGraph, &CHeuristicEuclid::instance, l_iOrigin, l_iDestination);

  list<int> l_Path = search.GetPathToTarget();

  list<int>::iterator l_it  = l_Path.begin();
  list<int>::iterator l_end = l_Path.end();

  for(;l_it != l_end; ++l_it)
  {
    Path_.push_back( &(m_pGraph->GetNode(*l_it)) );
  }

  return Path_;
}

vector<Vect3f>      CIAManager::SearchPathVec(const Vect3f& _vOrigin, const Vect3f& _vDestination)
{
  vector<CGraphNode*> l_Path = SearchPath(_vOrigin, _vDestination);
  vector<Vect3f> Path_;
  
  Path_.push_back(_vOrigin);

  vector<CGraphNode*>::iterator l_it  = l_Path.begin();
  vector<CGraphNode*>::iterator l_end = l_Path.end();

  for(;l_it != l_end; ++l_it)
  {
    Path_.push_back((*l_it)->GetPosition());
  }
  
  Path_.push_back(_vDestination);
  return Path_;
}


vector<CGraphNode*> CIAManager::GetClosestCobertura   (const Vect3f& _vPosition)
{
  vector<CGraphNode*> Path_;

  int l_iOrigin      = GetClosestNode(_vPosition);
  

  set<int> l_CoberturesLliures;
  {
    set<int>::iterator l_it  = m_NodesCobertura.begin();
    set<int>::iterator l_end = m_NodesCobertura.end();
    for(; l_it != l_end; ++l_it)
    {
      CGraphNode l_Node = m_pGraph->GetNode(*l_it);
      CGameEntity *l_pNodeEntity = l_Node.GetEntity();
      CComponentNavNode *l_pCNN = l_pNodeEntity->GetComponent<CComponentNavNode>();
      if(!l_pCNN->m_bOcupat)
      {
        l_CoberturesLliures.insert(*l_it); //El node de la cobertura
      }
    }
  }

  CSearchAStar search( *m_pGraph, &CHeuristicEuclid::instance, l_iOrigin, l_CoberturesLliures);

  list<int> l_Path = search.GetPathToTarget();

  {
    list<int>::iterator l_it  = l_Path.begin();
    list<int>::iterator l_end = l_Path.end();

    for(;l_it != l_end; ++l_it)
    {
      Path_.push_back( &(m_pGraph->GetNode(*l_it)) );
    }
  }

  return Path_;
}

vector<Vect3f>      CIAManager::GetClosestCoberturaVec(const Vect3f& _vPosition)
{
  vector<CGraphNode*> l_Path = GetClosestCobertura(_vPosition);
  vector<Vect3f> Path_;
  
  Path_.push_back(_vPosition);

  vector<CGraphNode*>::iterator l_it  = l_Path.begin();
  vector<CGraphNode*>::iterator l_end = l_Path.end();

  for(;l_it != l_end; ++l_it)
  {
    Path_.push_back((*l_it)->GetPosition());
  }
  return Path_;
}

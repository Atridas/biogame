#include "ComponentNavNode.h"

#include "Core.h"
#include "IAManager.h"
#include "GraphDefines.h"
#include "ComponentObject3D.h"

CComponentNavNode* CComponentNavNode::AddToEntity(CGameEntity* _pEntity, bool _bAutoroute, float _fMaxAutoDistance, const set<string>& _DefaultEdges)
{
  CComponentNavNode *l_pComp = new CComponentNavNode();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _bAutoroute, _fMaxAutoDistance, _DefaultEdges))
  {
    l_pComp->SetEntity(_pEntity);
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponentNavNode::Init(CGameEntity* _pEntity, bool _bAutoroute, float _fMaxAutoDistance, const set<string>& _DefaultEdges)
{
  CComponentObject3D* m_pCO3D = _pEntity->GetComponent<CComponentObject3D>();
  if(!m_pCO3D)
  {
    SetOk(false);
    LOGGER->AddNewLog(ELL_ERROR, "Component Nav Node needs Component Object 3D");
    return false;
  }

  CSparseGraph* l_pGraph = CORE->GetIAManager()->GetGraph();

  CGraphNode l_GraphNode;
  l_GraphNode.SetPosition(m_pCO3D->GetPosition());
  l_GraphNode.SetEntity(_pEntity);
  l_GraphNode.SetIndex(l_pGraph->GetNextFreeNodeIndex());

  l_pGraph->AddNode(l_GraphNode);
  m_iGraphNodeIndex = l_GraphNode.GetIndex();

  m_bAutoroute       = _bAutoroute;
  m_DefaultEdges     = _DefaultEdges;
  m_fMaxAutoDistance = _fMaxAutoDistance;

  SetOk(true);
  return IsOk();
}
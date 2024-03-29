#define __DONT_INCLUDE_MEM_LEAKS__
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Core.h"
#include "EntityDefines.h"
#include "IAManager.h"
#include "ScriptedStateMachine.h"
#include "GraphDefines.h"
#include "Heuristics.h"
#include "SearchAStar.h"


#include "Utils/MemLeaks.h"

using namespace luabind;

// ----------------------------------------------------

void RegisterIAToLua(lua_State* _pLS)
{
  module(_pLS) [

    class_<CIAManager, CBaseControl>("IAManager")
      .def("get_graph",         &CIAManager::GetGraph       )
      .def("get_closest_node",  (int(CIAManager::*)(const Vect3f&))      &CIAManager::GetClosestNode ) 
      .def("get_closest_node",  (int(CIAManager::*)(const Vect3f&, int)) &CIAManager::GetClosestNode ) 
      .def("search_path",       &CIAManager::SearchPath     )
      .def("search_path_vec",   &CIAManager::SearchPathVec  ) 

    ,class_<vector<CGraphNode*>>("GraphNodeVector")
      .def("size",              &vector<CGraphNode*>::size)
      .def("at",                (vector<CGraphNode*>::reference (vector<CGraphNode*>::*) (vector<CGraphNode*>::size_type))&vector<CGraphNode*>::at)
      .def("const_at",          (vector<CGraphNode*>::const_reference (vector<CGraphNode*>::*) (vector<CGraphNode*>::size_type) const)&vector<CGraphNode*>::at)
      
    ,class_<CGraphNode, CBaseControl>("GraphNode")
      .def(constructor<>())
      .def(constructor<int,const Vect3f&>())
      .def("get_index",    &CGraphNode::GetIndex)
      .def("get_position", &CGraphNode::GetPosition)
      .def("get_entity",   &CGraphNode::GetEntity)
      .def("set_index",    &CGraphNode::SetIndex)
      .def("set_position", &CGraphNode::SetPosition)
      .def("set_entity",   &CGraphNode::SetEntity)

    ,class_<CGraphEdge, CBaseControl>("GraphEdge")
      .def(constructor<>())
      .def(constructor<int,int>())
      .def(constructor<int,int,float>())
      .def("get_from",    &CGraphEdge::GetFrom )
      .def("get_to",      &CGraphEdge::GetTo   )
      .def("get_cost",    &CGraphEdge::GetCost )
      .def("set_from",    &CGraphEdge::SetFrom )
      .def("set_to",      &CGraphEdge::SetTo   )
      .def("set_cost",    &CGraphEdge::SetCost )

    ,class_<CSparseGraph>("SparseGraph")
      .def(constructor<bool>())
      .def("get_node"                 ,(const CGraphNode&(CSparseGraph::*)(int)const)&CSparseGraph::GetNode              ) // const CGraphNode&  GetNode(int idx)const;
      .def("get_node"                 ,(      CGraphNode&(CSparseGraph::*)(int)     )&CSparseGraph::GetNode              ) // CGraphNode&  GetNode(int idx);
      //No vui que es faci servir aquesta funci�, s'ha de fer servir la de l'IAManager
      //.def("get_closest_node"         ,&CSparseGraph::GetClosestNode       ) 
      .def("get_edge"                 ,(const CGraphEdge&(CSparseGraph::*)(int, int)const)&CSparseGraph::GetEdge              ) // const CGraphEdge& GetEdge(int from, int to)const;
      .def("get_edge"                 ,(      CGraphEdge&(CSparseGraph::*)(int, int)     )&CSparseGraph::GetEdge              ) // CGraphEdge& GetEdge(int from, int to);
      .def("get_next_free_node_index" ,&CSparseGraph::GetNextFreeNodeIndex ) 
      .def("add_node"                 ,&CSparseGraph::AddNode              )
      .def("remove_node"              ,&CSparseGraph::RemoveNode           )
      .def("add_edge"                 ,&CSparseGraph::AddEdge              )
      .def("remove_edge"              ,&CSparseGraph::RemoveEdge           )
      .def("set_edge_cost"            ,&CSparseGraph::SetEdgeCost          )
      .def("num_nodes"                ,&CSparseGraph::NumNodes             )
      .def("num_active_nodes"         ,&CSparseGraph::NumActiveNodes       )
      .def("num_edges"                ,&CSparseGraph::NumEdges             )
      .def("is_digraph"               ,&CSparseGraph::isDigraph            )
      .def("is_empty"                 ,&CSparseGraph::isEmpty              )
      .def("is_node_present"          ,&CSparseGraph::isNodePresent        )
      .def("is_edge_present"          ,&CSparseGraph::isEdgePresent        )
      .def("clear"                    ,&CSparseGraph::Clear                )
      .def("remove_edges"             ,&CSparseGraph::RemoveEdges          )
      
    ,class_<CSparseGraph::EdgeIterator>("EdgeIterator")
      .def(constructor<CSparseGraph&, int>())
      .def("begin",  &CSparseGraph::EdgeIterator::begin)
      .def("next",   &CSparseGraph::EdgeIterator::next)
      .def("end",    &CSparseGraph::EdgeIterator::end)
    ,class_<CSparseGraph::ConstEdgeIterator>("ConstEdgeIterator")
      .def(constructor<const CSparseGraph&, int>())
      .def("begin",  &CSparseGraph::ConstEdgeIterator::begin)
      .def("next",   &CSparseGraph::ConstEdgeIterator::next)
      .def("end",    &CSparseGraph::ConstEdgeIterator::end)
    ,class_<CSparseGraph::NodeIterator>("NodeIterator")
      .def(constructor<CSparseGraph&>())
      .def("begin",  &CSparseGraph::NodeIterator::begin)
      .def("next",   &CSparseGraph::NodeIterator::next)
      .def("end",    &CSparseGraph::NodeIterator::end)
    ,class_<CSparseGraph::ConstNodeIterator>("ConstNodeIterator")
      .def(constructor<const CSparseGraph&>())
      .def("begin",  &CSparseGraph::ConstNodeIterator::begin)
      .def("next",   &CSparseGraph::ConstNodeIterator::next)
      .def("end",    &CSparseGraph::ConstNodeIterator::end)

    //GraphDefines
    ,def("valid_neighbour", &ValidNeighbour)
    ,def("GraphHelper_add_all_neighbours_to_grid_node",&GraphHelper_AddAllNeighboursToGridNode)
    ,def("GraphHelper_create_grid",&GraphHelper_CreateGrid)
    
    //Heuristics
    ,class_<CHeuristic>("Heuristic")
      .def("calculate", &CHeuristic::Calculate)
    ,class_<CHeuristicEuclid, CHeuristic>("HeuristicEuclid")
      .def("calculate", &CHeuristicEuclid::Calculate)
    ,class_<CHeuristicNoisyEuclidian,CHeuristic>("HeuristicNoisyEuclidian")
      .def("calculate", &CHeuristicNoisyEuclidian::Calculate)
    ,class_<CHeuristicDijkstra>("HeuristicDijkstra")
      .def("calculate", &CHeuristicDijkstra::Calculate)
      
    ,def("get_heuristic_euclid",       &GetHeuristicEuclid)
    ,def("get_heuristic_noisy_euclid", &GetHeuristicNoisyEuclid)
    ,def("get_heuristic_dijkstra",     &GetHeuristicDijkstra)

    //SearchAStar
    ,class_<CSearchAStar>("SearchAStar")
      .def(constructor<const CSparseGraph&,const CHeuristic*,int,int>())
      .def("get_SPT"           ,  &CSearchAStar::GetSPT)
      .def("get_path_to_target",  &CSearchAStar::GetPathToTarget)
      .def("get_cost_to_target",  &CSearchAStar::GetCostToTarget)

    //Scripted State Machine
    ,class_<CScriptedStateMachine>("ScriptedStateMachine")
      .def("set_current_state",    &CScriptedStateMachine::SetCurrentState)
      .def("change_state",         &CScriptedStateMachine::ChangeState)
      .def("current_state",        &CScriptedStateMachine::CurrentState)
      .def("revert_state",         &CScriptedStateMachine::RevertState)
      .def("receive_event",        &CScriptedStateMachine::ReceiveEvent)
  ];
}

void RegisterCore_IA(class_<CCore>& _Core)
{
  _Core.def("get_ia_manager",              &CCore::GetIAManager);
}





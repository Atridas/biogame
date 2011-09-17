#pragma once
#ifndef __COMPONENT_NAV_NODE__
#define __COMPONENT_NAV_NODE__

#include "base.h"
#include "EntityDefines.h"

class CComponentNavNode:
  public CBaseComponent
{
public:

  ~CComponentNavNode() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_NAV_NODE;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_NAV_NODE;};

  static CComponentNavNode* AddToEntity(CGameEntity* _pEntity, bool _bAutoroute = true, float _fMaxAutoDistance = -1.f, const set<string>& _DefaultEdges = set<string>());

  int GetGraphNodeIndex() const { return m_iGraphNodeIndex; };

  bool m_bOcupat;
  bool m_bCoberturaBaixa;
  bool m_bAutoroute;
  float m_fMaxAutoDistance;
  set<string> m_DefaultEdges;

protected:

  CComponentNavNode():m_iGraphNodeIndex(-1),m_bAutoroute(0),m_bCoberturaBaixa(false),m_bOcupat(false) {};
  bool Init(CGameEntity* _pEntity, bool _bAutoroute, float _fMaxAutoDistance, const set<string>& _DefaultEdges);
  void Release() {m_DefaultEdges.clear();};

private:

  int m_iGraphNodeIndex;
};

#endif

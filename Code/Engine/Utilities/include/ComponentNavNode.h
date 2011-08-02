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

  static CComponentNavNode* AddToEntity(CGameEntity* _pEntity);

  int GetGraphNodeIndex() const { return m_iGraphNodeIndex; };

protected:

  CComponentNavNode():m_iGraphNodeIndex(-1) {};
  bool Init(CGameEntity* _pEntity);
  void Release() {};

private:

  int m_iGraphNodeIndex;
};

#endif

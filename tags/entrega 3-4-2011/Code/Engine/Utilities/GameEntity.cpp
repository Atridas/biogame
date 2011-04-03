#include "EntityDefines.h"

int g_LastGUID = 0;


void CGameEntity::Init() {
  m_iGUID = g_LastGUID++;

  SetOk(true);
};

void CGameEntity::Update(float deltaTime)
{
  map<CBaseComponent::EComponentType, CBaseComponent*>::iterator l_it = m_vComponents.begin();
  map<CBaseComponent::EComponentType, CBaseComponent*>::iterator l_end = m_vComponents.begin();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->Update(deltaTime);
  }
}

void CGameEntity::ReceiveEvent(const SEvent& _Event)
{
  map<CBaseComponent::EComponentType, CBaseComponent*>::iterator l_it = m_vComponents.begin();
  map<CBaseComponent::EComponentType, CBaseComponent*>::iterator l_end = m_vComponents.begin();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->ReceiveEvent(_Event);
  }
}

CBaseComponent* CGameEntity::GetComponent(CBaseComponent::EComponentType _type) const
{
  map<CBaseComponent::EComponentType, CBaseComponent*>::const_iterator l_it = m_vComponents.find(_type);

  if(l_it == m_vComponents.cend())
  {
    return 0;
  }
  return l_it->second;
}
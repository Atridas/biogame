#include "EntityDefines.h"


void CGameEntity::AddComponent(CBaseComponent* _pComponent)
{
  assert(m_vComponents.find(_pComponent->GetType()) == m_vComponents.end());
  m_vComponents[_pComponent->GetType()] = _pComponent;
}

void CGameEntity::Update(float deltaTime)
{
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_it = m_vComponents.begin();
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->Update(deltaTime);
  }
}

void CGameEntity::ReceiveEvent(const SEvent& _Event)
{
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_it = m_vComponents.begin();
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->ReceiveEvent(_Event);
  }
}

CBaseComponent* CGameEntity::GetComponent(CBaseComponent::Type _type) const
{
  map<CBaseComponent::Type, CBaseComponent*>::const_iterator l_it = m_vComponents.find(_type);

  if(l_it == m_vComponents.cend())
  {
    return 0;
  }
  return l_it->second;
}

void CGameEntity::Release()
{
  map<CBaseComponent::Type, CBaseComponent*>::const_iterator l_it = m_vComponents.begin();
  map<CBaseComponent::Type, CBaseComponent*>::const_iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    delete l_it->second;
  }

  m_vComponents.clear();
}
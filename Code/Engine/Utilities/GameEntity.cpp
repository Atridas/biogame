#include "EntityDefines.h"

#include <sstream>

void CGameEntity::AddComponent(CBaseComponent* _pComponent)
{
  assert(m_mComponents.find(_pComponent->GetType()) == m_mComponents.end());
  m_mComponents[_pComponent->GetType()] = _pComponent;

  //insertem els components en ordre de tipus.
  int position = m_vComponents.size();
  m_vComponents.push_back(_pComponent);
  while(position>0 && m_vComponents[position-1]->GetType() > _pComponent->GetType())
  {
    m_vComponents[position] = m_vComponents[position-1];
    m_vComponents[position-1] = _pComponent;
    --position;
  }
}

void CGameEntity::PreUpdate(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->PreUpdate(deltaTime);
  }
}

void CGameEntity::Update(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->Update(deltaTime);
  }
}

void CGameEntity::UpdatePrePhysX(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->UpdatePrePhysX(deltaTime);
  }
}

void CGameEntity::UpdatePostPhysX(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->UpdatePostPhysX(deltaTime);
  }
}

void CGameEntity::UpdatePostAnim(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->UpdatePostAnim(deltaTime);
  }
}

void CGameEntity::PostUpdate(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->PostUpdate(deltaTime);
  }
}

void CGameEntity::ReceiveEvent(const SEvent& _Event)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->ReceiveEvent(_Event);
  }
}

CBaseComponent* CGameEntity::GetComponent(CBaseComponent::Type _type) const
{
  map<CBaseComponent::Type, CBaseComponent*>::const_iterator l_it = m_mComponents.find(_type);

  if(l_it == m_mComponents.cend())
  {
    return 0;
  }
  return l_it->second;
}

string CGameEntity::GetName() const
{
  if(m_pszName)
  {
    return *m_pszName;
  } else {
    stringstream l_SStream;
    l_SStream << "Entity " << GetGUID();
    return l_SStream.str();
  }
}

void CGameEntity::Release()
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  
  m_mComponents.clear();
  m_vComponents.clear();
}
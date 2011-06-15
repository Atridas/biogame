#include "EntityDefines.h"

#include <sstream>

void CGameEntity::AddComponent(CBaseComponent* _pComponent)
{
  m_vNewEntities[_pComponent->GetType()] = _pComponent;
}

void CGameEntity::AddCachedComponents()
{
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_it  = m_vNewEntities.begin();
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_end = m_vNewEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = l_it->second;

    assert(m_mComponents.find(l_pComponent->GetType()) == m_mComponents.end());
    m_mComponents[l_pComponent->GetType()] = l_pComponent;

    //insertem els components en ordre de tipus.
    int position = m_vComponents.size();
    m_vComponents.push_back(l_pComponent);
    while(position>0 && m_vComponents[position-1]->GetType() > l_pComponent->GetType())
    {
      m_vComponents[position] = m_vComponents[position-1];
      m_vComponents[position-1] = l_pComponent;
      --position;
    }
  }

  m_vNewEntities.clear();
}

void CGameEntity::PreUpdate(float deltaTime)
{
  AddCachedComponents();

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

void CGameEntity::DebugRender(CRenderManager* _pRM)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->DebugRender(_pRM);
  }
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
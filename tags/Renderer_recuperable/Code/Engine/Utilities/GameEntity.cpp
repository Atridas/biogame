#include "EntityDefines.h"

#include <sstream>

void CGameEntity::AddComponent(CBaseComponent* _pComponent)
{
  assert(m_vNewComponents.find(_pComponent->GetType()) == m_vNewComponents.end());
  m_vNewComponents[_pComponent->GetType()] = _pComponent;
}

void CGameEntity::DeleteComponent(CBaseComponent::Type _type) 
{
  //assert(m_vDeleteComponents.find(_pComponent->GetType()) == m_vDeleteComponents.end());
  m_vDeleteComponents.insert(_type);
};

void CGameEntity::AddCachedComponents()
{
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_it  = m_vNewComponents.begin();
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_end = m_vNewComponents.end();

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

  m_vNewComponents.clear();
}

void CGameEntity::DeleteComponents()
{
  //TODO optimitzar!
  set<CBaseComponent::Type>::iterator l_it  = m_vDeleteComponents.begin();
  set<CBaseComponent::Type>::iterator l_end = m_vDeleteComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent::Type l_type = *l_it;

    for(uint32 i = 0; i < m_vComponents.size(); ++i)
    {
      if(m_vComponents[i]->GetType() > l_type)
      {
        break;
      } else if(m_vComponents[i]->GetType() == l_type)
      {
        delete m_vComponents[i];
        for(uint32 j = i; j + 1 < m_vComponents.size(); ++j)
        {
          m_vComponents[j] = m_vComponents[j+1];
        }
        m_vComponents.pop_back();
        m_mComponents.erase(l_type);
        break;
      }
    }
  }

  m_vDeleteComponents.clear();
}

void CGameEntity::PreUpdate(float deltaTime)
{
  AddCachedComponents();

  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = (*l_it);
    if(l_pComponent->IsActive())
    {
      l_pComponent->PreUpdate(deltaTime);
    }
  }
  DeleteComponents();
}

void CGameEntity::Update(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = (*l_it);
    if(l_pComponent->IsActive())
    {
      l_pComponent->Update(deltaTime);
    }
  }
  DeleteComponents();
}

void CGameEntity::UpdatePrePhysX(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = (*l_it);
    if(l_pComponent->IsActive())
    {
      l_pComponent->UpdatePrePhysX(deltaTime);
    }
  }
  DeleteComponents();
}

void CGameEntity::UpdatePostPhysX(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = (*l_it);
    if(l_pComponent->IsActive())
    {
      l_pComponent->UpdatePostPhysX(deltaTime);
    }
  }
  DeleteComponents();
}

void CGameEntity::UpdatePostAnim(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = (*l_it);
    if(l_pComponent->IsActive())
    {
      l_pComponent->UpdatePostAnim(deltaTime);
    }
  }
  DeleteComponents();
}

void CGameEntity::PostUpdate(float deltaTime)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = (*l_it);
    if(l_pComponent->IsActive())
    {
      l_pComponent->PostUpdate(deltaTime);
    }
  }
  DeleteComponents();
}

void CGameEntity::SetActive(bool _bActive)
{
  if(m_bActive == _bActive)
    return;

  m_bActive = _bActive;

  map<CBaseComponent::Type, CBaseComponent*>::iterator l_itMap  = m_vNewComponents.begin();
  map<CBaseComponent::Type, CBaseComponent*>::iterator l_endMap = m_vNewComponents.end();

  for(; l_itMap != l_endMap; ++l_itMap)
  {
    CBaseComponent* l_pComponent = l_itMap->second;
    l_pComponent->SetActive(m_bActive);
  }

  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = (*l_it);
    l_pComponent->SetActive(m_bActive);
  }
}

void CGameEntity::ReceiveEvent(const SEvent& _Event)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    CBaseComponent* l_pComponent = (*l_it);
    if(l_pComponent->IsActive())
    {
      l_pComponent->ReceiveEvent(_Event);
    }
  }
  DeleteComponents();
}

void CGameEntity::DebugRender(CRenderManager* _pRM)
{
  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    (*l_it)->DebugRender(_pRM);
  }
  DeleteComponents();
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
  AddCachedComponents();

  vector<CBaseComponent*>::iterator l_it = m_vComponents.begin();
  vector<CBaseComponent*>::iterator l_end = m_vComponents.end();

  for(; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  
  m_mComponents.clear();
  m_vComponents.clear();
}
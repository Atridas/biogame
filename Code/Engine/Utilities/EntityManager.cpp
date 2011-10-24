#include "EntityManager.h"

void CEntityManager::Release()
{
  CreateEntities();
  RemoveEntities();

  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }

  m_vEntities.clear();
  m_vFreeIDs.clear();
  m_vNames.clear();
  m_vEvents.clear();
}

CGameEntity* CEntityManager::CreateEntity()
{
  CGameEntity* l_pEntity;
  if(!m_vFreeIDs.empty())
  {
    int l_iId = m_vFreeIDs.back();
    m_vFreeIDs.pop_back();

    l_pEntity = new CGameEntity(l_iId);
    //m_vEntities[l_iId] = l_pEntity;
    m_vNewEntities.push_back(l_pEntity);
  } else {
    int l_iId = m_vEntities.size() + m_vNewEntities.size();

    l_pEntity = new CGameEntity(l_iId);
    //m_vEntities.push_back(l_pEntity);
    m_vNewEntities.push_back(l_pEntity);
  }
  return l_pEntity;
}

void CEntityManager::CreateEntities()
{
  vector<CGameEntity*>::iterator l_it  = m_vNewEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vNewEntities.end();
  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = *l_it;

    if((uint32)l_pEntity->GetGUID() >= m_vEntities.size())
    {
      m_vEntities.resize(l_pEntity->GetGUID()+1);
    }
    m_vEntities[l_pEntity->GetGUID()] = l_pEntity;
  }
  m_vNewEntities.clear();
}


CGameEntity* CEntityManager::GetEntity(int _iId) const
{
  assert(m_vEntities.size() > (uint32) _iId);
  for(uint32 i = 0; i < m_vFreeIDs.size(); ++i) assert(m_vFreeIDs[i] != _iId);

  return m_vEntities[_iId];
}


bool CEntityManager::SetName(const string& _szName,const CGameEntity* _pEntity)
{
  return SetName(_szName, _pEntity->GetGUID());
}

bool CEntityManager::SetName(const string& _szName, int _iId)
{
  assert(m_vEntities.size() + m_vNewEntities.size() > (uint32) _iId);
  for(uint32 i = 0; i < m_vFreeIDs.size(); ++i) assert(m_vFreeIDs[i] != _iId);
  if(m_vNames.find(_szName) != m_vNames.end())
  {
    return false;
  }

  m_vNames[_szName] = _iId;

  vector<CGameEntity*>::iterator l_it  = m_vNewEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vNewEntities.end();
  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = *l_it;
    if(l_pEntity->GetGUID() == _iId)
    {
      l_pEntity->m_pszName = &(m_vNames.find(_szName)->first);
      return true;
    }
  }

  m_vEntities[_iId]->m_pszName = &(m_vNames.find(_szName)->first); //TODO hi ha d'haver una manera més correcte de fer això
  return true;
}

CGameEntity* CEntityManager::GetEntity(const string& _szName) const
{
  map<string,int>::const_iterator l_it = m_vNames.find(_szName);

  if(l_it == m_vNames.cend())
  {
    return 0;
  } else {

    vector<CGameEntity*>::const_iterator l_it2  = m_vNewEntities.begin();
    vector<CGameEntity*>::const_iterator l_end2 = m_vNewEntities.end();
    for(; l_it2 != l_end2; ++l_it2)
    {
      CGameEntity* l_pEntity = *l_it2;
      if(l_pEntity->GetGUID() == l_it->second)
      {
        return l_pEntity;
      }
    }

    return m_vEntities[l_it->second];
  }
}


void CEntityManager::RemoveEntity(const CGameEntity* _pEntity)
{
  RemoveEntity(_pEntity->GetGUID());
}

void CEntityManager::RemoveEntity(int _iId)
{
  assert(m_vEntities.size() > (uint32) _iId);
  for(uint32 i = 0; i < m_vFreeIDs.size(); ++i)// assert(m_vFreeIDs[i] != _iId);
  {
    if(m_vFreeIDs[i] == _iId)
    {
      LOGGER->AddNewLog(ELL_ERROR, "CEntityManager::RemoveEntity intentant eliminar una entitat ja eliminada.");
      return;
    }
  }

  m_vDeleteEntities.push(_iId);
}

void CEntityManager::RemoveEntities()
{
  while(!m_vDeleteEntities.empty())
  {
    int l_iId = m_vDeleteEntities.top();
    m_vDeleteEntities.pop();
    assert(m_vEntities.size() > (uint32) l_iId);
    for(uint32 i = 0; i < m_vFreeIDs.size(); ++i) //assert(m_vFreeIDs[i] != l_iId);
    {
      if(m_vFreeIDs[i] == l_iId)
      {
        LOGGER->AddNewLog(ELL_ERROR, "CEntityManager::RemoveEntity intentant eliminar una entitat ja eliminada.");
        return;
      }
    }

  
    map<string,int>::iterator l_it  = m_vNames.begin();
    map<string,int>::iterator l_end = m_vNames.end();

    for(; l_it != l_end; ++l_it)
    {
      if(l_it->second == l_iId)
      {
        m_vNames.erase(l_it->first);
        break;
      }
    }

    delete m_vEntities[l_iId];
    m_vEntities[l_iId] = 0;
    m_vFreeIDs.push_back(l_iId);
  }
}




/// ----------------------------- UPDATES ----------------------------------------------

void CEntityManager::PreUpdate(float _fDeltaTime)
{
  CreateEntities();
  RemoveEntities();
  SendEvents(_fDeltaTime);

  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = (*l_it);
    if(l_pEntity)
    {
      if(l_pEntity->IsActive())
      {
        l_pEntity->PreUpdate(_fDeltaTime);
      }
    }
  }
}

void CEntityManager::Update(float _fDeltaTime)
{
  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = (*l_it);
    if(l_pEntity)
    {
      if(l_pEntity->IsActive())
      {
        l_pEntity->Update(_fDeltaTime);
      }
    }
  }
}

void CEntityManager::UpdatePrePhysX(float _fDeltaTime)
{
  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = (*l_it);
    if(l_pEntity)
    {
      if(l_pEntity->IsActive())
      {
        l_pEntity->UpdatePrePhysX(_fDeltaTime);
      }
    }
  }
}

void CEntityManager::UpdatePostPhysX(float _fDeltaTime)
{
  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = (*l_it);
    if(l_pEntity)
    {
      if(l_pEntity->IsActive())
      {
        l_pEntity->UpdatePostPhysX(_fDeltaTime);
      }
    }
  }
}

void CEntityManager::UpdatePostAnim(float _fDeltaTime)
{
  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = (*l_it);
    if(l_pEntity)
    {
      if(l_pEntity->IsActive())
      {
        l_pEntity->UpdatePostAnim(_fDeltaTime);
      }
    }
  }
}

void CEntityManager::PostUpdate(float _fDeltaTime)
{
  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = (*l_it);
    if(l_pEntity)
    {
      if(l_pEntity->IsActive())
      {
        l_pEntity->PostUpdate(_fDeltaTime);
      }
    }
  }

  CreateEntities();
  RemoveEntities();
}




/// ----------------------------- RENDER ----------------------------------------------


void CEntityManager::DebugRender(CRenderManager* _pRM)
{
  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = (*l_it);
    if(l_pEntity) 
      l_pEntity->DebugRender(_pRM);
  }
}
  


/// ----------------------------- EVENTS ----------------------------------------------


void CEntityManager::SendEvent(const SEvent& _Event)
{
  m_vEvents.push_back(_Event);
}

void CEntityManager::DeliverEvent(const SEvent& _Event)
{
  if(_Event.DispatchTime <= 0)
  {
    if(_Event.Receiver < 0)
    {
      vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
      vector<CGameEntity*>::iterator l_end = m_vEntities.end();
  
      for(; l_it != l_end; ++l_it)
      {
        CGameEntity* l_pEntity = (*l_it);
        if(l_pEntity)
        {
          if(l_pEntity->IsActive())
          {
            l_pEntity->ReceiveEvent(_Event);
          }
        }
      }
    }
    else
    {
      //TODO asserts i tal...
      CGameEntity* l_pEntity = m_vEntities[_Event.Receiver];

      if(l_pEntity)
      {
        if(l_pEntity->IsActive())
        {
          l_pEntity->ReceiveEvent(_Event);
        }
      }
    }
  }
}

void CEntityManager::SendEvents(float _fDeltaTime)
{
  for(uint32 i = 0; i < m_vEvents.size();)
  {
    m_vEvents[i].DispatchTime -= _fDeltaTime;
    if(m_vEvents[i].DispatchTime <= 0)
    {
      DeliverEvent(m_vEvents[i]);

      m_vEvents[i] = m_vEvents[m_vEvents.size() - 1];

      m_vEvents.pop_back();
    }
    else
    {
      ++i;
    }
  }
}
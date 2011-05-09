#include "EntityManager.h"

void CEntityManager::Release()
{
  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    delete *l_it;
  }
  
  m_vEntities.clear();
  m_vFreeIDs.clear();
  m_vNames.clear();
}

CGameEntity* CEntityManager::CreateEntity()
{
  CGameEntity* l_pEntity;
  if(!m_vFreeIDs.empty())
  {
    int l_iId = m_vFreeIDs.back();
    m_vFreeIDs.pop_back();

    l_pEntity = new CGameEntity(l_iId);
    m_vEntities[l_iId] = l_pEntity;
  } else {
    int l_iId = m_vEntities.size();

    l_pEntity = new CGameEntity(l_iId);
    m_vEntities.push_back(l_pEntity);
  }
  return l_pEntity;
}

CGameEntity* CEntityManager::GetEntity(int _iId) const
{
  assert(m_vEntities.size() < (uint32) _iId);
  for(uint32 i = 0; i < m_vFreeIDs.size(); ++i) assert(m_vFreeIDs[i] != _iId);

  return m_vEntities[_iId];
}


void CEntityManager::SetName(const string& _szName,const CGameEntity* _pEntity)
{
  SetName(_szName, _pEntity->GetGUID());
}

void CEntityManager::SetName(const string& _szName, int _iId)
{
  assert(m_vEntities.size() > (uint32) _iId);
  for(uint32 i = 0; i < m_vFreeIDs.size(); ++i) assert(m_vFreeIDs[i] != _iId);
  assert(m_vNames.find(_szName) == m_vNames.end());

  m_vNames[_szName] = _iId;
  m_vEntities[_iId]->m_pszName = &(m_vNames.find(_szName)->first); //TODO hi ha d'haver una manera més correcte de fer això
}

CGameEntity* CEntityManager::GetEntity(const string& _szName) const
{
  map<string,int>::const_iterator l_it = m_vNames.find(_szName);

  if(l_it == m_vNames.cend())
  {
    return 0;
  } else {
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
  for(uint32 i = 0; i < m_vFreeIDs.size(); ++i) assert(m_vFreeIDs[i] != _iId);

  
  map<string,int>::iterator l_it  = m_vNames.begin();
  map<string,int>::iterator l_end = m_vNames.end();

  for(; l_it != l_end; ++l_it)
  {
    if(l_it->second == _iId)
    {
      m_vNames.erase(l_it->first);
      break;
    }
  }

  delete m_vEntities[_iId];
  m_vEntities[_iId] = 0;
  m_vFreeIDs.push_back(_iId);
}




/// ----------------------------- UPDATES ----------------------------------------------

void CEntityManager::PreUpdate(float _fDeltaTime)
{
  vector<CGameEntity*>::iterator l_it  = m_vEntities.begin();
  vector<CGameEntity*>::iterator l_end = m_vEntities.end();

  for(; l_it != l_end; ++l_it)
  {
    CGameEntity* l_pEntity = (*l_it);
    if(l_pEntity) 
      l_pEntity->PreUpdate(_fDeltaTime);
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
      l_pEntity->Update(_fDeltaTime);
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
      l_pEntity->UpdatePrePhysX(_fDeltaTime);
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
      l_pEntity->UpdatePostPhysX(_fDeltaTime);
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
      l_pEntity->UpdatePostAnim(_fDeltaTime);
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
      l_pEntity->PostUpdate(_fDeltaTime);
  }
}

#include "GameEntity.h"

int g_LastGUID = 0;


void CGameEntity::Init() {
  m_iGUID = g_LastGUID++;

  SetOk(true);
};

void CGameEntity::Update(float deltaTime)
{
  map<EComponentType, CBaseComponent*>::iterator l_it = m_vComponents.begin();
  map<EComponentType, CBaseComponent*>::iterator l_end = m_vComponents.begin();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->Update(deltaTime);
  }
}

CBaseComponent* CGameEntity::GetComponent(EComponentType _type) const
{
  map<EComponentType, CBaseComponent*>::const_iterator l_it = m_vComponents.find(_type);

  if(l_it == m_vComponents.cend())
  {
    return 0;
  }
  return l_it->second;
}
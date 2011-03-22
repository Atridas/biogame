#include "GameEntity.h"

void CGameEntity::Update(float deltaTime)
{
  for(uint32 i = 0; i < m_vComponents.size(); ++i)
  {
    m_vComponents[i]->Update(deltaTime);
  }
}
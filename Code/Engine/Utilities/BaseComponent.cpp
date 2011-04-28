#include "EntityDefines.h"

void CBaseComponent::SetEntity(CGameEntity* _pEntity) 
{
  m_pEntity = _pEntity; _pEntity->AddComponent(this);
};
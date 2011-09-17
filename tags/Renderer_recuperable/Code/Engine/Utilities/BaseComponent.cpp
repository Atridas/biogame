#include "EntityDefines.h"

void CBaseComponent::SetEntity(CGameEntity* _pEntity) 
{
  m_pEntity = _pEntity; _pEntity->AddComponent(this);
};

void CBaseComponent::SetActive(bool _bActive)
{
  m_bActive = _bActive;
  
  if(m_bActive)
  {
    Enable();
  }else{
    Disable();
  }
}
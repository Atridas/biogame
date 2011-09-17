#include "ComponentMovement.h"


CComponentMovement* CComponentMovement::AddToEntity(CGameEntity *_pEntity)
{
  CComponentMovement *l_pComp = new CComponentMovement();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity))
  {
    l_pComp->SetEntity(_pEntity);
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponentMovement::Init(CGameEntity *_pEntity)
{
  SetOk(true);
  return IsOk();
}

void CComponentMovement::PreUpdate(float _fDeltaTime)
{
  m_vMovement.Set(0, 0, 0);
}

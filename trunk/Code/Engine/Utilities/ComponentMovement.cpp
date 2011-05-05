#include "ComponentMovement.h"

bool CComponentMovement::Init(CGameEntity *_pEntity)
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  SetOk(true);
  return IsOk();
}

void CComponentMovement::PreUpdate(float _fDeltaTime)
{
  m_vMovement.Set(0, 0, 0);
}

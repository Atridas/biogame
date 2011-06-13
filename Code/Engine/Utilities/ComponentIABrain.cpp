#include "ComponentIABrain.h"
#include "Core.h"


bool CComponentIABrain::Init(CGameEntity* _pEntity, const string& _szPlayerEntityName)
{
  assert(_pEntity);
  SetEntity(_pEntity);

  m_pPlayer = CORE->GetEntityManager()->GetEntity(_szPlayerEntityName);

  SetOk(true);
  return IsOk();
}
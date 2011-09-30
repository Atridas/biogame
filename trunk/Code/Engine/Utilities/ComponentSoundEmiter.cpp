#include "ComponentSoundEmiter.h"

CComponentSoundEmiter* CComponentSoundEmiter::AddToEntity(CGameEntity* _pEntity)
{
  CComponentSoundEmiter *l_pComp = new CComponentSoundEmiter();
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

bool CComponentSoundEmiter::Init(CGameEntity* _pEntity)
{
  SetOk(true);
  return IsOk();
}

void CComponentSoundEmiter::Release()
{
}
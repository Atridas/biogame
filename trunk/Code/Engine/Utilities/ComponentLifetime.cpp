#include "ComponentLifetime.h"
#include "Core.h"


CComponentLifetime* CComponentLifetime::AddToEntity(CGameEntity *_pEntity, float _fTime)
{
  CComponentLifetime *l_pComp = new CComponentLifetime();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fTime))
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

bool CComponentLifetime::Init(CGameEntity *_pEntity, float _fTime)
{
  assert(_fTime >= 0);

  m_fTime = _fTime;

  SetOk(true);
  return IsOk();
}

void CComponentLifetime::Update(float _fDeltaTime) 
{
  m_fTime -= _fDeltaTime;
  if(m_fTime <= 0)
  {
    ENTITY_MANAGER->RemoveEntity( GetEntity() );
  }
}

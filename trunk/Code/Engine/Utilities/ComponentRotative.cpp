#include "ComponentRotative.h"
#include "ComponentPhysXActor.h"
#include "ComponentTrigger.h"

CComponentRotative* CComponentRotative::AddToEntity(CGameEntity *_pEntity, const Vect3f& _vRotation)
{
  CComponentRotative *l_pComp = new CComponentRotative();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _vRotation))
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

//yaw pitch roll
bool CComponentRotative::Init(CGameEntity* _pEntity, const Vect3f& _vRotation)
{
  m_vRotation = Vect3f(_vRotation.y, _vRotation.x, _vRotation.z);

  m_pActor = _pEntity->GetComponent<CComponentPhysXActor>();

  if(!m_pActor)
    m_pActor = _pEntity->GetComponent<CComponentTrigger>();

  if(m_pActor)
  {
    SetOk(true);
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentRotative::Init No s'ha trobat cap actor físic.");
    SetOk(false);
  }

  return IsOk();
}

void CComponentRotative::UpdatePrePhysX(float _fDeltaTime)
{
  if(m_pActor)
  {
    m_pActor->SetRotation(m_pActor->GetRotation() + m_vRotation * _fDeltaTime);
  }
}
#include "ComponentRotative.h"
#include "ComponentRenderableObject.h"
#include "ComponentTrigger.h"

CComponentRotative* CComponentRotative::AddToEntity(CGameEntity *_pEntity, float _fYawRotation, float _fPitchRotation, float _fRollRotation)
{
  CComponentRotative *l_pComp = new CComponentRotative();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fYawRotation, _fPitchRotation, _fRollRotation))
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
bool CComponentRotative::Init(CGameEntity* _pEntity, float _fYawRotation, float _fPitchRotation, float _fRollRotation)
{
  m_fYawRotation   = _fYawRotation;
  m_fPitchRotation = _fPitchRotation;
  m_fRollRotation  = _fRollRotation;

  m_pCRO = _pEntity->GetComponent<CComponentRenderableObject>();

  if(m_pCRO)
  {
    SetOk(true);
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentRotative::Init No s'ha trobat cap component renderable object.");
    SetOk(false);
  }

  return IsOk();
}

void CComponentRotative::PreUpdate(float _fDeltaTime)
{
  if(m_pCRO)
  {
    m_pCRO->m_fYawAdjustment += m_fYawRotation * _fDeltaTime;
    if(m_pCRO->m_fYawAdjustment >  FLOAT_PI_VALUE) m_pCRO->m_fYawAdjustment -= 2 * FLOAT_PI_VALUE;
    if(m_pCRO->m_fYawAdjustment < -FLOAT_PI_VALUE) m_pCRO->m_fYawAdjustment += 2 * FLOAT_PI_VALUE;

    m_pCRO->m_fPitchAdjustment += m_fPitchRotation * _fDeltaTime;
    if(m_pCRO->m_fPitchAdjustment >  FLOAT_PI_VALUE) m_pCRO->m_fPitchAdjustment -= 2 * FLOAT_PI_VALUE;
    if(m_pCRO->m_fPitchAdjustment < -FLOAT_PI_VALUE) m_pCRO->m_fPitchAdjustment += 2 * FLOAT_PI_VALUE;

    m_pCRO->m_fRollAdjustment += m_fRollRotation * _fDeltaTime;
    if(m_pCRO->m_fRollAdjustment >  FLOAT_PI_VALUE) m_pCRO->m_fRollAdjustment -= 2 * FLOAT_PI_VALUE;
    if(m_pCRO->m_fRollAdjustment < -FLOAT_PI_VALUE) m_pCRO->m_fRollAdjustment += 2 * FLOAT_PI_VALUE;
  }
}
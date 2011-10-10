#include "ComponentEnergy.h"
#include "Core.h"

CComponentEnergy* CComponentEnergy::AddToEntity(CGameEntity* _pEntity, float _fInitialEnergy, float _fMaxEnergy, float _fRegenAmount, float _fTimeForRegen)
{
  CComponentEnergy *l_pComp = new CComponentEnergy();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fInitialEnergy, _fMaxEnergy, _fRegenAmount, _fTimeForRegen))
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

bool CComponentEnergy::Init(CGameEntity* _pEntity, float _fInitialEnergy, float _fMaxEnergy, float _fRegenAmount, float _fTimeForRegen)
{
  m_fEnergy = _fInitialEnergy;
  m_fMaxEnergy = _fMaxEnergy;
  m_fRegenAmount = _fRegenAmount;
  m_fTimeForRegen = _fTimeForRegen;
  m_fTimeSinceUsed = 0.0f;

  SetOk(true);
  return IsOk();
}

void CComponentEnergy::Update(float _fDeltaTime)
{  
  if(m_fTimeSinceUsed <= m_fTimeForRegen)
  {
    m_fTimeSinceUsed += _fDeltaTime;
  }else{
    Increase(m_fRegenAmount * _fDeltaTime);
  }
}

void CComponentEnergy::Increase(float _fAmount)
{
  if(m_fEnergy < m_fMaxEnergy)
  {
    if(_fAmount < 0)
      _fAmount *= -1;

    m_fEnergy += _fAmount;

    if(m_fEnergy > m_fMaxEnergy)
      m_fEnergy = m_fMaxEnergy;
  }
}

bool CComponentEnergy::Decrease(float _fAmount)
{
  assert(_fAmount >= 0.0f);

  if(m_fEnergy >= _fAmount)
  {
    m_fTimeSinceUsed = 0.0f;
    m_fEnergy -= _fAmount;

    return true;
  }
  else
  {
    return false;
  }

}

void CComponentEnergy::Set(float _fAmount)
{
  m_fEnergy = _fAmount;

  if(m_fEnergy > m_fMaxEnergy)
    m_fEnergy = m_fMaxEnergy;
  else if(m_fEnergy < 0.0f)
    m_fEnergy = 0.0f;
}

void CComponentEnergy::SetRegen(float _fRegenAmount)
{
  assert(_fRegenAmount >= 0);
  m_fRegenAmount = _fRegenAmount;
}

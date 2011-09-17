#include "ComponentShield.h"
#include "Core.h"

CComponentShield* CComponentShield::AddToEntity(CGameEntity* _pEntity, float _fHP, float _fMaxHP, float _fRegenAmount, float _fCooldown)
{
  CComponentShield *l_pComp = new CComponentShield();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fHP, _fMaxHP, _fRegenAmount, _fCooldown))
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

bool CComponentShield::Init(CGameEntity* _pEntity, float _fHP, float _fMaxHP, float _fRegenAmount, float _fCooldown)
{
  m_fHP = _fHP;
  m_fMaxHP = _fMaxHP;
  m_fRegenAmount = _fRegenAmount;
  m_fCooldown = _fCooldown;
  m_fTime = m_fCooldown;

  //m_bActive = false;

  SetOk(true);
  return IsOk();
}

void CComponentShield::Update(float _fDeltaTime)
{
  /*if(m_fTime <= m_fCooldown)
  {
    m_fTime += _fDeltaTime;
  }else if(!m_bActive)
  {
    Increase(m_fRegenAmount * _fDeltaTime);
  }*/
}

void CComponentShield::Increase(float _fAmount)
{
  if(m_fHP < m_fMaxHP)
  {
    if(_fAmount < 0)
      _fAmount *= -1;

    m_fHP += _fAmount;

    if(m_fHP > m_fMaxHP)
      m_fHP = m_fMaxHP;
  }
}

float CComponentShield::Decrease(float _fAmount)
{
  /*if(m_bActive)
  {
    if(_fAmount < 0.0f)
        _fAmount *= -1;

    //L'impacte trenca l'escut
    float l_fDamage = _fAmount - m_fHP;

    if(m_fHP > 0.0f)
    {
      m_fHP -= _fAmount;

      if(m_fHP < 0.0f)
      {
        //escut desactivat
        m_fHP = 0.0f;
        m_fTime = 0.0f;
        m_bActive = false;
        return l_fDamage;
      }
      return 0.0f;
    }
  }*/

  return _fAmount;
}

void CComponentShield::Set(float _fAmount)
{
  m_fHP = _fAmount;

  if(m_fHP > m_fMaxHP)
    m_fHP = m_fMaxHP;
  else if(m_fHP < 0.0f)
    m_fHP = 0.0f;
}

void CComponentShield::SetRegen(float _fRegenAmount)
{
  if(_fRegenAmount >= 0.0f)
    m_fRegenAmount = _fRegenAmount;
}

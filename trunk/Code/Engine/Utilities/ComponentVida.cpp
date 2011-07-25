#include "ComponentVida.h"
#include "ComponentShield.h"
#include "Core.h"

CComponentVida* CComponentVida::AddToEntity(CGameEntity* _pEntity, float _fVidaInicial, float _fVidaMaxima, bool _bRegen, float _fRegenAmount)
{
  CComponentVida *l_pComp = new CComponentVida();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _fVidaInicial, _fVidaMaxima, _bRegen, _fRegenAmount))
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

bool CComponentVida::Init(CGameEntity* _pEntity, float _fVidaInicial, float _fVidaMaxima, bool _bRegen, float _fRegenAmount)
{
  m_fVida = _fVidaInicial;
  m_fVidaMaxima = _fVidaMaxima;
  m_bRegen = _bRegen;
  m_fRegenAmount = _fRegenAmount;

  m_pShield = _pEntity->GetComponent<CComponentShield>(ECT_SHIELD);

  SetOk(true);
  return IsOk();
}

void CComponentVida::Update(float _fDeltaTime)
{
  if(m_bRegen)
    Increase(m_fRegenAmount * _fDeltaTime);
}

void CComponentVida::Increase(float _fAmount)
{
  if(m_fVida < m_fVidaMaxima)
  {
    if(_fAmount < 0)
      _fAmount *= -1;

    m_fVida += _fAmount;

    if(m_fVida > m_fVidaMaxima)
      m_fVida = m_fVidaMaxima;
  }
}

void CComponentVida::Decrease(float _fAmount)
{
  if(!m_bImmortal && m_fVida > 0.0f)
  {
    //TODO: check escuts a l'entitat
    if(_fAmount < 0.0f)
      _fAmount *= -1;

    m_fVida -= _fAmount;

    if(m_fVida < 0.0f)
      m_fVida = 0.0f;
  }
}

void CComponentVida::Set(float _fAmount)
{
  m_fVida = _fAmount;

  if(m_fVida > m_fVidaMaxima)
    m_fVida = m_fVidaMaxima;
  else if(m_fVida < 0.0f)
    m_fVida = 0.0f;
}

void CComponentVida::SetRegen(float _fRegenAmount)
{
  if(_fRegenAmount >= 0.0f)
  {
    m_fRegenAmount = _fRegenAmount;

    if(m_fRegenAmount = 0.0f)
      m_bRegen = false;
    else
      m_bRegen = true;
  }
}

void CComponentVida::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::REBRE_IMPACTE)
  {
    assert(_Event.Info[0].Type == SEventInfo::FLOAT);

    if(m_fVida > 0.f)
    {
      float l_fDamage = _Event.Info[0].f;

      //Si té escut, el mal el rep aquest.
      //La resta del mal passa al jugador per si l'escut es trenca.
      if(m_pShield)
        l_fDamage = m_pShield->Decrease(l_fDamage);

      Decrease(l_fDamage);

      if(m_fVida <= 0.f)
      {
        SEvent l_morir;
        l_morir.Msg = SEvent::MORIR;
        l_morir.Receiver = l_morir.Sender = GetEntity()->GetGUID();
      
        CORE->GetEntityManager()->SendEvent(l_morir);
      }
    }
  }
}